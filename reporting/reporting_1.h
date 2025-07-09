/** 
 * Reporitng:
 * 
 * There are three levels of reporting. Level 1 is for routine events, such as regular program functioning.
 * Level 2 is for events that are signifigant but only truly useful for a developer or contributor.
 * Level 3 is VERY RARE, monumentious events that require immediete reporting, like undefined behavior 
 * or errors.
 * The standard macros used to activate these levels are:
 * 
 *  - REPORTING_1
 * 
 *  - REPORTING_2
 *
 *  - REPORTING_3
 *
 * These files are intended s simple ways to manage the level of reporting for each file. 
 * reporting_1.h defines REPORTING_1, REPORTING_2, REPORTING_3, reporting_2.h defines REPORTING_1, 
 * REPORTING_2, and reporting_3.h defines REPORTING_3. reporting_none.h is a file that defines
 * defines no macros.
 */

 #define REPORTING_1