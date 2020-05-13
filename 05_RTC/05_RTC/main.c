#include <atmel_start.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structs for Calendar Alarms
static struct calendar_alarm alarm1;
static struct calendar_alarm alarm2;

// Struct for Timer Task
static struct timer_task task;

/**
 * Callback for Timer Task
 *
 */
static void timer_task_cb(const struct timer_task *const timer_task)
{
	// Toggle LED
	gpio_toggle_pin_level(LED);
}

/**
 * Callback for Alarm
 *
 */
static void alarm_cb(struct calendar_descriptor *const descr)
{
	// Datetime Object
	struct calendar_date_time datetime;
	
	// Get date and time from calendar
	calendar_get_date_time(&CALENDAR, &datetime);
	
	// Allocate Memory for Debug Out String
	char *alarm_message = malloc(32);
	
	// Format Alarm Message
	sprintf(alarm_message, "ALARM AT %d-%d-%d %02d:%02d:%02d\r\n", datetime.date.year, datetime.date.month, datetime.date.day, datetime.time.hour, datetime.time.min, datetime.time.sec);
	
	// Write to Debug Out
	io_write(&DEBUGOUT, alarm_message, strlen(alarm_message));
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Enable UART
	usart_sync_enable(&DEBUGOUT);
	
	// Set up Calendar Date and Time Structs
	struct calendar_date date;
	struct calendar_time time;
	
	// Enable the Calendar
	calendar_enable(&CALENDAR);
	
	// Set the Date
	date.year = 2019;
	date.month = 10;
	date.day = 11;
	
	// Set the Time
	time.hour = 18;
	time.min = 0;
	time.sec = 0;
	
	// Set date and time on calendar
	calendar_set_date(&CALENDAR, &date);
	calendar_set_time(&CALENDAR, &time);
	
	// Set up alarms
	alarm1.cal_alarm.datetime.time.sec = 5;
	alarm1.cal_alarm.option = CALENDAR_ALARM_MATCH_SEC;
	alarm1.cal_alarm.mode = REPEAT;
	alarm2.cal_alarm.datetime.time.sec = 25;
	alarm2.cal_alarm.option = CALENDAR_ALARM_MATCH_SEC;
	alarm2.cal_alarm.mode = ONESHOT;
	
	// Set the Alarms
	calendar_set_alarm(&CALENDAR, &alarm1, alarm_cb);
	calendar_set_alarm(&CALENDAR, &alarm2, alarm_cb);
	
	// Set up Timer Function
	task.interval = 1;
	task.cb = timer_task_cb;
	task.mode = TIMER_TASK_REPEAT;
	
	// Add timer task
	timer_add_task(&TIMER, &task);
	timer_start(&TIMER);

	/* Replace with your application code */
	while (1) {
	}
}
