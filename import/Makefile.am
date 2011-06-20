pkgsysconfdir 	= $(sysconfdir)/$(PACKAGE)
importdir 	= ${pkgsysconfdir}/import

# Seed imports for questionare food tables
import_DATA = \
	frequencies.csv 	\
	weights.csv		\
	portions.csv 		\
	meals.csv 		\
	foods.csv 		\
	nutrients.csv 		\
	meal_foods.csv 		\
	food_nutrients.csv 	\
	cereals.csv 		\
	milks.csv

EXTRA_DIST = $(import_DATA)



# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(importdir)


