/*
 * trace.h
 *
 *  Created on: Jun 3, 2020
 *      Author: bdouram
 */

#ifndef SRC_TRACE_TRACE_MACROS_H_
#define SRC_TRACE_TRACE_MACROS_H_

void xTraceTaskReady(char *task);
void xTraceTaskIn(char *task);
void xTraceTaskOut(char *task);

void xTraceTaskInherit();
void xTraceTaskEnd();
void xTraceTaskDisinherit();

#define traceMOVED_TASK_TO_READY_STATE(xTask) 					xTraceTaskReady(xTask->pcTaskName)
#define traceREADDED_TASK_TO_READY_STATE(xTask)  				xTraceTaskReady(xTask->pcTaskName)

#define traceTASK_SWITCHED_IN() 								xTraceTaskIn(pxCurrentTCB->pcTaskName)
#define traceTASK_SWITCHED_OUT() 								xTraceTaskOut(pxCurrentTCB->pcTaskName)

#define traceTASK_PRIORITY_INHERIT(pxTCB,uxPriority)			xTraceTaskInherit()
#define traceTASK_DELAY_UNTIL(xTimeToWake)						xTraceTaskEnd()
#define traceTASK_PRIORITY_DISINHERIT(pxTCB,uxBasePriority)		xTraceTaskDisinherit()


#endif /* SRC_TRACE_TRACE_MACROS_H_ */
