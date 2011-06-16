# surely there is a better way then repeating declarations
confdir = $(sysconfdir)/$(PACKAGE)/conf

check_PROGRAMS = \
	test_person \
	test_frequencies \
	test_questionaire \
	test_meal \
	test_food \
	test_cereals \
	test_nutrient \
	test_meal_foods \
	test_questionaire_person \
	test_weights \
	test_food_nutrient \
	test_portions

BUILD_DEFS	=  -ggdb -O0 -DDEFAULT_CONFIG_FILE="\"${confdir}/client.conf\""
LINK_DEPS	= \
		   $(top_builddir)/src/libhelper/libEpicHelper.la 	\
		   $(top_builddir)/src/libdao/libEpicDAO.la

AM_CPPFLAGS 	= \
	$(BUILD_DEFS) \
        -I$(top_srcdir)/include   \
        -I$(top_srcdir)/src

AM_LDFLAGS	=  $(LINK_DEPS)

test_person_LDADD = $(LINK_DEPS)
test_person_DEPENDENCIES  = $(LINK_DEPS)
test_person_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	${top_srcdir}/include/dao/Person.hpp \
	${top_srcdir}/include/dao/Weight.hpp \
	test_person.cpp

test_frequencies_LDADD = $(LINK_DEPS)
test_frequencies_DEPENDENCIES  = $(LINK_DEPS)
test_frequencies_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/src/libdao/util.h \
	${top_srcdir}/include/config/Global.hpp \
	${top_srcdir}/include/dao/Frequency.hpp \
	test_frequencies.cpp

test_questionaire_LDADD = $(LINK_DEPS)
test_questionaire_DEPENDENCIES  = $(LINK_DEPS)
test_questionaire_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_questionaire.cpp

test_meal_LDADD = $(LINK_DEPS)
test_meal_DEPENDENCIES  = $(LINK_DEPS)
test_meal_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_meal.cpp

test_food_LDADD = $(LINK_DEPS)
test_food_DEPENDENCIES  = $(LINK_DEPS)
test_food_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_food.cpp

test_cereals_LDADD = $(LINK_DEPS)
test_cereals_DEPENDENCIES  = $(LINK_DEPS)
test_cereals_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_cereals.cpp

test_nutrient_LDADD = $(LINK_DEPS)
test_nutrient_DEPENDENCIES  = $(LINK_DEPS)
test_nutrient_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_nutrient.cpp

test_meal_foods_LDADD = $(LINK_DEPS)
test_meal_foods_DEPENDENCIES  = $(LINK_DEPS)
test_meal_foods_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_meal_foods.cpp

test_questionaire_person_LDADD = $(LINK_DEPS)
test_questionaire_person_DEPENDENCIES  = $(LINK_DEPS)
test_questionaire_person_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_questionaire_person.cpp

test_weights_LDADD = $(LINK_DEPS)
test_weights_DEPENDENCIES  = $(LINK_DEPS)
test_weights_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	${top_srcdir}/src/libdao/util.h \
	test_weights.cpp

test_food_nutrient_LDADD = $(LINK_DEPS)
test_food_nutrient_DEPENDENCIES  = $(LINK_DEPS)
test_food_nutrient_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_food_nutrient.cpp

test_portions_LDADD = $(LINK_DEPS)
test_portions_DEPENDENCIES  = $(LINK_DEPS)
test_portions_SOURCES = \
	${top_srcdir}/src/libdao/Database.hpp \
	${top_srcdir}/include/config/Global.hpp \
	test_portions.cpp

reallyclean: maintainer-clean
	rm -f Makefile
	rm -f Makefile.in
	rm -f  $(check_PROGRAMS)
