## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
      -I$(top_srcdir)/include   	\
      -I$(top_srcdir)/src

noinst_LTLIBRARIES = libEpicDAO.la

libEpicDAO_la_SOURCES = \
			Database.cpp      	\
			Database.hpp  	\
			Statement.hpp  	\
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
			$(top_srcdir)/include/conversion/Conversion.hpp	\
			\
			$(top_srcdir)/include/Singleton.hpp  \
			\
			$(top_srcdir)/src/libhelper/Logger.cpp \
			$(top_srcdir)/src/libhelper/Logger.hpp

libEpicDAO_la_LDFLAGS =
# -lpthread -lrt -lltdl

libEpicDAO_la_LIBADD = \
	$(top_builddir)/src/libsqlite/libSqlite.la
	$(top_builddir)/src/libcsv/libEpicCSV.la
