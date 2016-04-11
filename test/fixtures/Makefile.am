SUBDIRS = sql import conf .

fixturedir = $(builddir)
fixture_DATA = \
		$(fixturedir)/test.conf

CLEANFILES= $(fixture_DATA)

EXTRA_DIST=\
		$(fixturedir)/test.conf.in \
		$(fixture_DATA)

%.conf : %.conf.in
	sed -e 's![@]fixtures[@]!$(abs_builddir)!g' \
	$< > $@

$(fixturedir)/test.conf.in: $(top_builddir)/stamp-h1
	printf "%s\n" '# Location of the nutrients database' > $@
	printf "%s\n" 'database=":memory:"' >> $@
	printf "%s\n" '# create pure in memory database, replace with a file name to use a file db' >> $@
	printf "%s\n" 'schema="@fixtures@/sql/schema.sql"' >> $@
	printf "%s\n" '# nutrient quantities may contain codes which refer to missing data so an additional' >> $@
	printf "%s\n" '# config file helps with mapping the codes onto quantities, the absence of a value' >> $@
	printf "%s\n" '# in the config file is treated as if the value was present and mapped to its literal value.' >> $@
	printf "%s\n" 'nutrient_quantity="@fixtures@/conf/quantity.conf"' >> $@
	printf "%s\n" '# Meals import file - these are grid questions' >> $@
	printf "%s\n" 'meals="@fixtures@/import/meals.csv"' >> $@
	printf "%s\n" '# Food codes import file ' >> $@
	printf "%s\n" 'foods="@fixtures@/import/foods.csv"' >> $@
	printf "%s\n" '# Nutrients import file' >> $@
	printf "%s\n" 'nutrients="@fixtures@/import/nutrients.csv"' >> $@
	printf "%s\n" '# Conversion between frequency codes and multipliers' >> $@
	printf "%s\n" 'frequencies="@fixtures@/import/frequencies.csv"' >> $@
	printf "%s\n" '# Weighting factors ' >> $@
	printf "%s\n" 'weights="@fixtures@/import/weights.csv"' >> $@
	printf "%s\n" '# Milk portions' >> $@
	printf "%s\n" 'portions="@fixtures@/import/portions.csv"' >> $@
	printf "%s\n" '# Joins between meals and food tables' >> $@
	printf "%s\n" 'meal_foods="@fixtures@/import/meal_foods.csv"' >> $@
	printf "%s\n" '# Joins between foods and nutrients tables' >> $@
	printf "%s\n" 'food_nutrients="@fixtures@/import/food_nutrients.csv"' >> $@
	printf "%s\n" '# cereals, joins between foods and weights ' >> $@
	printf "%s\n" 'cereals="@fixtures@/import/cereals.csv"' >> $@
	printf "%s\n" '# milks' >> $@
	printf "%s\n" 'milks="@fixtures@/import/milks.csv"' >> $@
