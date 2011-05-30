## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\"  \
	-DSQLITE_OMIT_LOAD_EXTENSION=1	\
	-DSQLITE_THREADSAFE=0		\
        -I$(top_srcdir)/include   	\
        -I$(top_srcdir)/src/libdao   	\
        -I$(top_srcdir)/src  		\
        -I$(top_srcdir)/include/boost   \
        -I$(top_srcdir)/include/import  \
        -I$(top_srcdir)/include/pattern \
        -I$(top_srcdir)/include/conversion

noinst_LTLIBRARIES = libEpicDAO.la
libEpicDAO_la_SOURCES = \
 		   $(top_srcdir)/include/dataset/Database.hpp  \
                   $(top_srcdir)/include/dataset/Statement.hpp  \
 		   ${top_srcdir}/src/Database.cpp      	\
		   sqlite3.h		\
		   sqlite3.c		\
		   util.h		\
		   util.c		\
		   \
 		   ${top_srcdir}/src/FrequencyDAO.cpp	\
                   ${top_srcdir}/src/WeightDAO.cpp	\
                   ${top_srcdir}/src/FoodDAO.cpp		\
                   ${top_srcdir}/src/CerealDAO.cpp	\
                   ${top_srcdir}/src/MealDAO.cpp		\
                   ${top_srcdir}/src/MilkDAO.cpp		\
                   ${top_srcdir}/src/MealFoodDAO.cpp	\
                   ${top_srcdir}/src/NutrientDAO.cpp	\
                   ${top_srcdir}/src/PersonDAO.cpp	\
                   ${top_srcdir}/src/MealNutrientDAO.cpp	\
                   ${top_srcdir}/src/ReportDAO.cpp	\
                   ${top_srcdir}/src/PortionDAO.cpp	\
                   ${top_srcdir}/src/QuestionaireDAO.cpp	\
		   \
		   ${top_srcdir}/include/dao/FrequencyDAO.hpp	\
                   ${top_srcdir}/include/dao/ReportDAO.hpp	\
                   ${top_srcdir}/include/dao/WeightDAO.hpp	\
                   ${top_srcdir}/include/dao/MilkDAO.hpp		\
                   ${top_srcdir}/include/dao/CerealDAO.hpp	\
                   ${top_srcdir}/include/dao/PortionDAO.hpp	\
                   ${top_srcdir}/include/dao/FoodDAO.hpp		\
		   ${top_srcdir}/include/dao/MealNutrientDAO.hpp	\
                   ${top_srcdir}/include/dao/MealFoodDAO.hpp	\
                   ${top_srcdir}/include/dao/MealDAO.hpp		\
                   ${top_srcdir}/include/dao/NutrientDAO.hpp	\
                   ${top_srcdir}/include/dao/PersonDAO.hpp	\
                   ${top_srcdir}/include/dao/QuestionaireDAO.hpp	\
		   \
 		   $(top_srcdir)/include/dao/Frequency.hpp	\
 		   $(top_srcdir)/include/dao/Report.hpp		\
 		   $(top_srcdir)/include/dao/Weight.hpp		\
 		   $(top_srcdir)/include/dao/Milk.hpp		\
 		   $(top_srcdir)/include/dao/Cereal.hpp		\
 		   $(top_srcdir)/include/dao/Portion.hpp		\
 		   $(top_srcdir)/include/dao/Food.hpp		\
 		   $(top_srcdir)/include/dao/FoodNutrient.hpp	\
                   $(top_srcdir)/include/dao/MealNutrient.hpp	\
 		   $(top_srcdir)/include/dao/MealFood.hpp		\
 		   $(top_srcdir)/include/dao/Meal.hpp		\
 		   $(top_srcdir)/include/dao/Nutrient.hpp		\
 		   $(top_srcdir)/include/dao/Person.hpp		\
                   $(top_srcdir)/include/dao/Questionaire.hpp	\
                   \
		   $(top_srcdir)/include/config/Util.hpp  \
                   \
                   $(top_srcdir)/include/pattern/Singleton.hpp  \
                   \
		   $(top_srcdir)/src/logger.cpp

libEpicDAO_la_LDFLAGS = 
 # -lpthread -lrt -lltdl

libEpicDAO_la_LIBADD = 
#$(top_builddir)/libutility/src/libUtility.la
