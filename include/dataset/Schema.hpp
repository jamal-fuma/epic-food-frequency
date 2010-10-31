#ifndef EPIC_SCHEMA_HPP
#define EPIC_SCHEMA_HPP

#define EPIC_DB_SCHEMA ( \
"BEGIN TRANSACTION; "  \
\
"CREATE TABLE IF NOT EXISTS baking_fats ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "line INTEGER NOT NULL, "  \
    "food VARCHAR NOT NULL, "  \
    "amount REAL NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS baking_fats_food_index on baking_fats(food);" \
\
"CREATE TABLE IF NOT EXISTS migrations ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "version VARCHAR NOT NULL, "  \
    "description VARCHAR NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS questionaires ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code varchar NOT NULL, "  \
    "filename VARCHAR NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS questions ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "question_type_id INTEGER NOT NULL, "  \
    "description varchar NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS respondents ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "reference varchar NOT NULL, "  \
    "questionaire_id INTEGER NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS responses ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "value varchar NOT NULL, "  \
    "question_id INTEGER NOT NULL, "  \
    "respondent_id INTEGER NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS modifiers ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "line INTEGER NOT NULL, "  \
    "code VARCHAR NOT NULL, "  \
    "amount REAL NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS cereal_brands ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code INTEGER UNIQUE NOT NULL, "  \
    "description varchar NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS cereal_brands_alternatives ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "cereal_brand_id INTEGER UNIQUE NOT NULL, "  \
    "description varchar NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS cereal_types ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code INTEGER UNIQUE NOT NULL, "  \
    "food VARCHAR NOT NULL, "  \
    "description VARCHAR NOT NULL,"  \
    "weight_id INTEGER NOT NULL "  \
"); "  \
"CREATE TABLE IF NOT EXISTS cereals_type_alternatives ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "cereal_type_id INTEGER NOT NULL, "  \
    "description VARCHAR NOT NULL "  \
"); "  \
"CREATE TABLE IF NOT EXISTS cereals ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "line INTEGER NOT NULL, "  \
    "amount REAL NOT NULL "  \
"); "  \
\
"CREATE TABLE IF NOT EXISTS foods ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code VARCHAR NOT NULL, "  \
    "nutrient INTEGER NOT NULL, "  \
    "amount REAL NOT NULL, "  \
    "UNIQUE(code,nutrient) "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS foods_code_index on foods(code);" \
"CREATE UNIQUE INDEX IF NOT EXISTS foods_nutrient_index on foods(code,nutrient);" \
\
"CREATE TABLE IF NOT EXISTS frequencies ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code INTEGER UNIQUE NOT NULL, "  \
    "amount REAL NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS frequencies_line_index on frequencies(code);" \
\
"CREATE TABLE IF NOT EXISTS frying_fats ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "line INTEGER NOT NULL, "  \
    "amount REAL NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS frying_fats_line_index on frying_fats(line);" \
\
"CREATE TABLE IF NOT EXISTS groups ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code INTEGER UNIQUE NOT NULL, "  \
    "description varchar NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS groups_code_index on groups(code);" \
\
"CREATE TABLE IF NOT EXISTS ingredients ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "meal_id INTEGER NOT NULL, "  \
    "food VARCHAR NOT NULL, "  \
    "amount INTEGER NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS ingredients_meal_line_index on ingredients(meal_id,food);" \
\
"CREATE TABLE IF NOT EXISTS meals ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "line INTEGER UNIQUE NOT NULL, "  \
    "name VARCHAR NOT NULL, "  \
    "description VARCHAR NOT NULL"  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS meal_name_index on meals(name);" \
"CREATE INDEX IF NOT EXISTS meal_line_index on meals(line);" \
\
"CREATE TABLE IF NOT EXISTS milk_types ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "food VARCHAR NOT NULL, "  \
    "description VARCHAR NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS milk_types_food_index on milk_types(food);" \
\
"CREATE TABLE IF NOT EXISTS milk_type_alternatives ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "milk_type_id INTEGER NOT NULL, "  \
    "description VARCHAR NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS milk_types_alt_type_index on milk_type_alternatives(milk_type_id);" \
\
"CREATE TABLE IF NOT EXISTS milk_portions ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code INTEGER UNIQUE NOT NULL, "  \
    "amount REAL NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS milk_portion_code_index on milk_portions(code);" \
\
"CREATE TABLE IF NOT EXISTS nutrients ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code INTEGER UNIQUE NOT NULL, "  \
    "description VARCHAR NOT NULL, "  \
    "units VARCHAR NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS nutrients_code_index on nutrients(code);" \
\
"CREATE TABLE IF NOT EXISTS question_types ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "description varchar NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS question_types_id_index on question_types(id);" \
\
"CREATE TABLE IF NOT EXISTS visible_fats ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "meal_id INTEGER NOT NULL, "  \
    "food VARCHAR NOT NULL, "  \
    "amount REAL NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS visible_fats_meal_index on visible_fats(meal_id);" \
\
"CREATE TABLE IF NOT EXISTS weights ("  \
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "  \
    "code INTEGER UNIQUE NOT NULL, "  \
    "amount REAL NOT NULL "  \
"); "  \
\
"CREATE INDEX IF NOT EXISTS weights_code_index on weights(code);" \
\
"COMMIT;"  \
"ANALYZE;" \
)

#endif /* ndef EPIC_DB_SCHEMA_HPP */
