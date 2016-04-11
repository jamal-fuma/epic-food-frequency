This text file is to document data testing conditions
-----------------------------------------------------

1.  free text (to be converted into food codes)
    - milk_other, cerial1, type1, cerial2, type2, fat_frying_type, fat_baking_type

2.  multiple entries in free text columns (to be converted into ';' seperated food codes)
    - milk_other, cerial1, type1, cerial2, type2, fat_frying_type, fat_baking_type

3.  negative codes
    - first 130 columns

4.  multiple ticks
    - first 130 columns, milk, milk_other, fat_frying, fat_frying_type, fat_baking, fat_baking_type

5.  don't eat porridge i.e. porridge='1'
    but mentioned porridge in cerial(1/2) OR type(1/2)

6.  don't eat breakfast i.e. eat_breakfast='2'
    but free-text entered in cerial(1/2) OR type(1/2)

7.  don't eat other breakfast cereal i.e. cerial='1'
    but free-text entered  in cerial(1/2) OR type(1/2)

8.  baking fat is not 'margarine' i.e. fat_baking is not equal to '5'
    but free-text entered in fat_baking_type

9.  baking fat is not 'margarine' i.e. fat_baking is not equal to '5'
    but multiple ticks and/or free-text entered in fat_baking_type

10. frying fat is not 'vegetable oil' i.e. fat_frying is not equal to '3'
    but free-text entered in fat_frying_type

11. frying fat is not 'vegetable oil' i.e. fat_frying is not equal to '3'
    but multiple ticks and/or free-text entered in fat_frying_type

12. multiple ticks(-4) entered in column: milk and/or
    multiple ticks/free-text entered in column: milk_other

*************************************************************************************

MILK
----

Check No.: 1

ID: 0310680V
MILK = 3 (i.e. dried milk -> skimmed milk)
MILK_OTHER = FARM MILK (i.e. whole milk)
MILK_FREQ=4 (i.e. 3/4 of a pint)
The FFQ script has assigned food code: 12001 for this participant
i.e. the ticked option is selected and free-text is ignored

Check No.: 2

If MILK=-4 and no free text is available,
then "standard milk" is assumed.
Checked for ID: 0320077J

Check No.: 3

ID: 0210656Y & 0110228M
If MILK_FREQUENCY=-9 and MILK=(1; 3,5)
then assumed that no milk is consumed i.e. no standard milk quantity is applied

Check No.: 4

ID: 0130165H
If MILK_FREQUENCY=1 (none) and one or more MILK options are selected
then assumed that no milk is consumed i.e. no standard milk quantity is applied

To Check: 1
If MILK=-9 (missing), no free text is available and MILK_FREQUENCY ne -9 or >1 (not missing/not consumed), then assume "standard milk".

To Check: 2
If more than one "MILK" options are selected and more than one
"MILK_FREQUENCY" options are selected then make decisions for these
two questions independently i.e.

1. MILK -> average of selected milk types
2. MILK_FREQUENCY -> average

**************************************************************************************

CEREALS
-------

Check No.:1

If CERIAL>1 and EAT_BREAKFAST=1, and free-text is available,
the the free text is mapped to a food code to get the nutrient data.
Currently, a maximum of two distinct cerial types (from possible 30)
irrespective of the no. of cereals mentioned in the FFQ.

To Check: 1

As above, but where free text is not available,
then "standard cereal" should be selected.


Check No.:2
ID: 0220134A
PORRIDGE=1 (i.e. never or less than once/month)
CERIAL=3 (i.e. once a week) and free text contains porridge and other cereals
the current program assigns frequency of 3 to porridge (treats porridge as a cereal)

Suggestion 1: all free text mapped to food code: 11018 i.e. porridge
should be dropped before calculating nutrients.

Suggestion 2: If more than two cereal types (e.g. 4) are mentioned in the FFQ,
then nutrient data should be obtained from (1/4)th of all these cereal types.

Check No.3:
ID: 0330014K
If CERIAL>1 and EAT_BREAKFAST=2 and no free text is available then "standard cereal" is assumed.

ID: 0310024G
If CERIAL>1 and EAT_BREAKFAST=2 and free text is available then free text is mapped to food codes in order to obtain nutrient data.


Check No.:4
ID: 0120041D
CERIAL=1 and free-text is available, program assumes that no cereal is consumed.
It doesn't matter if EAT_BREAKFAST=1 (yes)

*****************************************************************************************

BAKING FATS
-----------

For commercial/readymade foods, 17020 is always selected as baking fat.

Check No.:1
If FAT_BAKING=-9 and no free-text is available, then "standard oil" is assumed

Check No.:2
ID: 0110072S
FAT_FRY=1(i.e. butter) and free-text is "OLIVE", so the program uses butter
in calculationg nutrient values for: chips & roast potatoes

Check No.:3
ID: 0210382A
FAT_BAKING ne '5' i.e. margarine and free-text is available, then free-text is used to
calculate nutrient values.

Check No.:4
ID: 0410614T
If multiple ticks were selected for FAT_BAKING (3,4), the current program selects "combination of one oil and n/s fat" i.e. a7026.

To Check:
ID: 0210081Y
FAT_BAKING=6 (i.e. none) and free-text is not available
The program seems to use some unknown very-low fat type to calculate nutrients
- there were three baking items consumed i.e. meal_id=58,60,74 and fat_code=-500
we've 3.108 kcal coming from food_code=17020 but cannot link this with raw data!

Suggestion 1: if FAT_BAKING=6 (i.e. none) and free-text is not available, then don't use fat in homemade baking items
Suggestion 2: if FAT_BAKING=6 (i.e. none) and free-text is available, then use free-text to map baking fat and use it in homemade baking items.

Check No.:5
ID: 0210077T
If multiple ticks were selected for FAT_BAKING (3 GRAPESEED, 5), the current program
selects "grapeseed oil"

Suggestion 1: In the above mentioned situation, the new program should map "3 GRAPESEED"
to grapeseed oil and 5 to "margarine n/s" and use average of the two to calculate nutrient values.

*****************************************************************************************

FRYING FATS
-----------

ID: 0210081Y
FAT_FRYING=6 (i.e. none) but free-text is available(olive oil)
The program uses free-text to calculate nutrients for chips & roast potatoes

ID: 0210483K
FAT_FRYING=6 (i.e. none) and free-text is not available
The program seems to use some unknown very-low fat type to calculate nutrients
We've worked out the nutrient(kcal) for standard fat in roast potatoes using food code=17020
but we cannot work-out modified fat (from back of FFQ) nutrient calculations for chips and roast potatoes
