CREATE TABLE baking_fats (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, line INTEGER NOT NULL, food VARCHAR NOT NULL, amount REAL NOT NULL );
CREATE TABLE cereal_brands (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code INTEGER UNIQUE NOT NULL, description varchar NOT NULL );
CREATE TABLE cereal_brands_alternatives (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, cereal_brand_id INTEGER UNIQUE NOT NULL, description varchar NOT NULL );
CREATE TABLE cereal_types (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code INTEGER UNIQUE NOT NULL, food VARCHAR NOT NULL, description VARCHAR NOT NULL,weight_id INTEGER NOT NULL );
CREATE TABLE cereals (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, line INTEGER NOT NULL, amount REAL NOT NULL );
CREATE TABLE cereals_type_alternatives (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, cereal_type_id INTEGER NOT NULL, description VARCHAR NOT NULL );
CREATE TABLE food_names (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code varchar);
CREATE TABLE food_nutrients (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, food_id integer, nutrient_id integer, quantity float);
CREATE TABLE foods (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code VARCHAR NOT NULL, nutrient INTEGER NOT NULL, amount REAL NOT NULL, UNIQUE(code,nutrient) );
CREATE TABLE frequencies (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code INTEGER UNIQUE NOT NULL, amount REAL NOT NULL );
CREATE TABLE frying_fats (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, line INTEGER NOT NULL, amount REAL NOT NULL );
CREATE TABLE groups (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code INTEGER UNIQUE NOT NULL, description varchar NOT NULL );
CREATE TABLE ingredients (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, meal_id INTEGER NOT NULL, food VARCHAR NOT NULL, amount INTEGER NOT NULL, modifier VARCHAR NOT NULL DEFAULT "", UNIQUE(meal_id,food) );
CREATE TABLE meal_foods (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, meal_id integer, food_id integer, quantity float);
CREATE TABLE meals (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, line INTEGER UNIQUE NOT NULL, name VARCHAR NOT NULL, description VARCHAR NOT NULL);
CREATE TABLE migrations (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, version VARCHAR NOT NULL, description VARCHAR NOT NULL );
CREATE TABLE milk_portions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code INTEGER UNIQUE NOT NULL, amount REAL NOT NULL );
CREATE TABLE milk_type_alternatives (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, milk_type_id INTEGER NOT NULL, description VARCHAR NOT NULL );
CREATE TABLE milk_types (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, food VARCHAR NOT NULL, description VARCHAR NOT NULL );
CREATE TABLE modifiers (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, line INTEGER NOT NULL, code VARCHAR NOT NULL, amount REAL NOT NULL );
CREATE TABLE nutrients (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code INTEGER UNIQUE NOT NULL, description VARCHAR NOT NULL, units VARCHAR NOT NULL );
CREATE TABLE question_types (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, description varchar NOT NULL );
CREATE TABLE questionaires (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code varchar NOT NULL, filename VARCHAR NOT NULL );
CREATE TABLE questions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, question_type_id INTEGER NOT NULL, description varchar NOT NULL );
CREATE TABLE respondents (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, reference varchar UNIQUE NOT NULL, questionaire_id INTEGER NOT NULL );
CREATE TABLE responses (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, value varchar NOT NULL, question_id INTEGER NOT NULL, respondent_id INTEGER NOT NULL );
CREATE TABLE visible_fats (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, meal_id INTEGER NOT NULL, food VARCHAR NOT NULL, amount REAL NOT NULL );
CREATE TABLE weights (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, code INTEGER UNIQUE NOT NULL, amount REAL NOT NULL );
CREATE INDEX baking_fats_food_index on baking_fats(food);
CREATE INDEX foods_code_index on foods(code);
CREATE UNIQUE INDEX foods_nutrient_index on foods(code,nutrient);
CREATE INDEX frequencies_line_index on frequencies(code);
CREATE INDEX frying_fats_line_index on frying_fats(line);
CREATE INDEX groups_code_index on groups(code);
CREATE INDEX ingredients_meal_line_index on ingredients(meal_id,food);
CREATE UNIQUE INDEX meal_foods_index on meal_foods(meal_id,food_id);
CREATE INDEX meal_line_index on meals(line);
CREATE INDEX meal_name_index on meals(name);
CREATE INDEX milk_portion_code_index on milk_portions(code);
CREATE INDEX milk_types_alt_type_index on milk_type_alternatives(milk_type_id);
CREATE INDEX milk_types_food_index on milk_types(food);
CREATE INDEX nutrients_code_index on nutrients(code);
CREATE INDEX question_types_id_index on question_types(id);
CREATE INDEX questions_description_index on questions(description);
CREATE INDEX respondents_reference_and_questionaire_index on respondents(reference,questionaire_id);
CREATE UNIQUE INDEX respondents_reference_index on respondents(reference);
CREATE UNIQUE INDEX response_respondent_index on responses(respondent_id,id);
CREATE INDEX visible_fats_meal_index on visible_fats(meal_id);
CREATE INDEX weights_code_index on weights(code);
