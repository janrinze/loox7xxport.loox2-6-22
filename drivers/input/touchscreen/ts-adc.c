/* Support for touchscreen attached to an ADC device.
 *
 * (c) Copyright 2006 Kevin O'Connor <kevin@koconnor.net>
 *
 * This file may be distributed under the terms of the GNU GPL license.
 */

#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/adc.h>
#include <linux/touchscreen-adc.h>

#include <asm/mach-types.h>
#include <asm/arch/hardware.h>
#include <asm/arch/pxa-regs.h>

struct adc_work {
	struct delayed_work work;
	struct platform_device *pdev;
};

struct ts_adc {
	struct input_dev *input;
	// Is touchscreen active.
	int active;

	struct adc_request req;
	struct adc_sense pins[4];

	struct workqueue_struct *wq;
	struct adc_work work;
};

#define	TS_POLL_PERIOD	msecs_to_jiffies(10)

// Send an event to the input layer.
static inline void
report_event(struct ts_adc *ts, int pressure, int x, int y)
{
	input_report_key(ts->input, BTN_TOUCH, pressure != 0);
	input_report_abs(ts->input, ABS_PRESSURE, pressure);
	if (pressure) {
		input_report_abs(ts->input, ABS_X, x);
		input_report_abs(ts->input, ABS_Y, y);
	}
	input_sync(ts->input);
}

// Handle values read from ADC.
static void
handle_adc(struct work_struct *work)
{
	struct delayed_work *delayed_work = container_of(work,
	                                         struct delayed_work, work);
	struct adc_work *adc_work = container_of(delayed_work,
	                                         struct adc_work, work);
	struct platform_device *pdev = adc_work->pdev;
	struct tsadc_platform_data *pdata = pdev->dev.platform_data;
	struct ts_adc *ts = platform_get_drvdata(pdev);
	u16 x, y, z1, z2, pendown;
	uint Rt = 0;

	adc_request_sample(&ts->req);

	// Calculate x, y, and pressure.
	x = ts->req.senses[0].value;
	y = ts->req.senses[1].value;
	z1 = ts->req.senses[2].value;
	z2 = ts->req.senses[3].value;

	// RTOUCH = (RXPlate) x (XPOSITION /4096) x [(Z2/Z1) - 1]
	if (likely(z1)) {
		Rt = z2;
		Rt -= z1;
		Rt *= x;
		Rt *= pdata->x_plate_ohms;
		Rt /= z1;
		Rt /= pdata->max_sense;
	}

	pendown = pdata->ispendown();

//	printk(KERN_NOTICE "TS: pen=%d x=%d y=%d Rt=%d (z1=%d z2=%d)\n"
//	       , pendown, x, y, Rt, z1, z2);

	if (!pendown || Rt < pdata->min_pressure) {
		// Pen no longer down.
		if (ts->active == 2)
			// Send pen up event.
			report_event(ts, 0, 0, 0);
		ts->active = 0;
		enable_irq(pdata->pen_irq);
	} else {
		// Pen down - arrange for events until pen up again.
		report_event(ts, Rt, x, y);
		ts->active = 2;
		queue_delayed_work(ts->wq, &ts->work.work, TS_POLL_PERIOD);
	}
}

// Interrupt handler for "pen down" events.
static irqreturn_t ts_penirq(int irq, void *handle)
{
	struct platform_device *pdev = handle;
	struct tsadc_platform_data *pdata = pdev->dev.platform_data;
	struct ts_adc *ts = platform_get_drvdata(pdev);
	disable_irq_nosync(pdata->pen_irq);
	if (! ts->active) {
		ts->active = 1;
		cancel_delayed_work(&ts->work.work);
		queue_delayed_work(ts->wq, &ts->work.work, 0);
	}
	return IRQ_HANDLED;
}

static int ts_probe(struct platform_device *pdev)
{
	struct ts_adc *ts;
	struct tsadc_platform_data *pdata = pdev->dev.platform_data;
	struct input_dev *input_dev;
	int irq_flags;
	int ret;

	// Initialize ts data structure.
	ts = kzalloc(sizeof(*ts), GFP_KERNEL);
	if (!ts)
		return -ENOMEM;

	platform_set_drvdata(pdev, ts);

	// Register interrupt
	if (pdata->pin_irq_falling)
		irq_flags = IRQF_TRIGGER_FALLING;
	else
		irq_flags = IRQF_TRIGGER_RISING;
	ret = request_irq(pdata->pen_irq, ts_penirq
			  , IRQF_DISABLED | irq_flags
			  , pdev->name, pdev);
	if (ret) {
		kfree(ts);
		return ret;
	}

	// Setup sense requests.
	ts->req.senses = ts->pins;
	ts->req.num_senses = ARRAY_SIZE(ts->pins);
	ts->pins[0].name = pdata->x_pin;
	ts->pins[1].name = pdata->y_pin;
	ts->pins[2].name = pdata->z1_pin;
	ts->pins[3].name = pdata->z2_pin;
	adc_request_register(&ts->req);

	// Setup work.
	ts->work.pdev = pdev;
	INIT_DELAYED_WORK(&ts->work.work, handle_adc);
	ts->wq = create_workqueue(pdev->dev.bus_id);
	if (!ts->wq) {
		free_irq(pdata->pen_irq, pdev);
		kfree(ts);
		return -ESRCH;
	}

	// Initialize input device.
	input_dev = input_allocate_device();
	input_dev->name = pdev->name;
	set_bit(EV_KEY, input_dev->evbit);
	set_bit(EV_ABS, input_dev->evbit);
	set_bit(ABS_X, input_dev->absbit);
	set_bit(ABS_Y, input_dev->absbit);
	set_bit(ABS_PRESSURE, input_dev->absbit);
	input_set_abs_params(input_dev, ABS_X, 0, pdata->max_sense, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, pdata->max_sense, 0, 0);
	input_set_abs_params(input_dev, ABS_PRESSURE, 0, pdata->max_sense
			     , 0, 0);
	input_register_device(input_dev);
	ts->input = input_dev;

	return 0;
}

static int ts_remove(struct platform_device *pdev)
{
	struct ts_adc *ts = platform_get_drvdata(pdev);
	struct tsadc_platform_data *pdata = pdev->dev.platform_data;

	free_irq(pdata->pen_irq, pdev);
	cancel_delayed_work(&ts->work.work);
	destroy_workqueue(ts->wq);
	adc_request_unregister(&ts->req);
	input_unregister_device(ts->input);
	kfree(ts);

	return 0;
}

static struct platform_driver ts_driver = {
	.driver = {
		.name           = "ts-adc",
	},
	.probe          = ts_probe,
	.remove         = ts_remove,
};

static int __init ts_init(void)
{
	return platform_driver_register(&ts_driver);
}

static void __exit ts_exit(void)
{
	platform_driver_unregister(&ts_driver);
}

module_init(ts_init)
module_exit(ts_exit)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kevin O'Connor <kevin@koconnor.net>");
