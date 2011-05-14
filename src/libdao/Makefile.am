## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\"  \
	-DSQLITE_OMIT_LOAD_EXTENSION=1	\
	-DSQLITE_THREADSAFE=0		\
        -I$(top_srcdir)/include   	\
        -I$(top_srcdir)/include/boost   \
        -I$(top_srcdir)/include/import  \
        -I$(top_srcdir)/include/pattern \
        -I$(top_srcdir)/include/logging \
        -I$(top_srcdir)/include/conversion

noinst_LTLIBRARIES = libEpicDAO.la
libEpicDAO_la_SOURCES = \
 		   Database.hpp  	\
                   Statement.hpp  	\
                   Database.cpp      	\
		   sqlite3.h		\
		   sqlite3.c		\
		   util.h		\
		   util.c		\
		   \
 		   FrequencyDAO.cpp	\
                   WeightDAO.cpp	\
                   FoodDAO.cpp		\
                   CerealDAO.cpp	\
                   MealDAO.cpp		\
                   MilkDAO.cpp		\
                   MealFoodDAO.cpp	\
                   NutrientDAO.cpp	\
                   PersonDAO.cpp	\
                   MealNutrientDAO.cpp	\
                   ReportDAO.cpp	\
                   PortionDAO.cpp	\
                   QuestionaireDAO.cpp	\
		   \
		   FrequencyDAO.hpp	\
                   ReportDAO.hpp	\
                   WeightDAO.hpp	\
                   MilkDAO.hpp		\
                   CerealDAO.hpp	\
                   PortionDAO.hpp	\
                   FoodDAO.hpp		\
		   MealNutrientDAO.hpp	\
                   MealFoodDAO.hpp	\
                   MealDAO.hpp		\
                   NutrientDAO.hpp	\
                   PersonDAO.hpp	\
                   QuestionaireDAO.hpp	\
		   \
 		   Frequency.hpp	\
 		   Report.hpp		\
 		   Weight.hpp		\
 		   Milk.hpp		\
 		   Cereal.hpp		\
 		   Portion.hpp		\
 		   Food.hpp		\
 		   FoodNutrient.hpp	\
                   MealNutrient.hpp	\
 		   MealFood.hpp		\
 		   Meal.hpp		\
 		   Nutrient.hpp		\
 		   Person.hpp		\
                   Questionaire.hpp	\
                   \
			$(top_srcdir)/include/config/Util.hpp  \
                   \
                   $(top_srcdir)/include/pattern/Singleton.hpp  \
                   \
		   $(top_srcdir)/include/logging/Logger.hpp

libEpicDAO_la_LDFLAGS = 
 # -lpthread -lrt -lltdl

libEpicDAO_la_LIBADD = 
#$(top_builddir)/libutility/src/libUtility.la
