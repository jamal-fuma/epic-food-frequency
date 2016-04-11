importdir=$(builddir)

import_DATA = \
	$(importdir)/frequencies.csv 		\
	$(importdir)/weights.csv		\
	$(importdir)/portions.csv 		\
	$(importdir)/meals.csv 			\
	$(importdir)/foods.csv 			\
	$(importdir)/nutrients.csv 		\
	$(importdir)/meal_foods.csv 		\
	$(importdir)/food_nutrients.csv 	\
	$(importdir)/cereals.csv 		\
	$(importdir)/milks.csv

CLEANFILES= $(import_DATA)

EXTRA_DIST=\
	 $(import_DATA)

$(importdir)/meals.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/meals.csv > $@

$(importdir)/foods.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/foods.csv > $@

$(importdir)/nutrients.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/nutrients.csv > $@

$(importdir)/frequencies.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/frequencies.csv > $@

$(importdir)/weights.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/weights.csv > $@

$(importdir)/portions.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/portions.csv > $@

$(importdir)/meal_foods.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/meal_foods.csv > $@

$(importdir)/food_nutrients.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/food_nutrients.csv > $@

$(importdir)/cereals.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/cereals.csv > $@

$(importdir)/milks.csv: $(abs_top_builddir)/stamp-h1
	cat  $(abs_top_srcdir)/import/milks.csv > $@
