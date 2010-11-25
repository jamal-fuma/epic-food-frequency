CREATE TABLE questionaires (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	filename VARCHAR UNIQUE NOT NULL
);

CREATE TABLE people (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	reference VARCHAR UNIQUE NOT NULL
);

CREATE TABLE meals (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	name VARCHAR UNIQUE NOT NULL,
	description VARCHAR NOT NULL
);
CREATE UNIQUE INDEX index_meals_on_name ON meals(name);

CREATE TABLE nutrients (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	code INTEGER UNIQUE NOT NULL,
	description VARCHAR NOT NULL,
	units VARCHAR NOT NULL
);
CREATE UNIQUE INDEX index_nutrients_on_code ON nutrients(code);

CREATE TABLE weights (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        amount REAL NOT NULL
);

CREATE TABLE frequencies (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        amount REAL NOT NULL
);

CREATE TABLE foods (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	name VARCHAR UNIQUE NOT NULL,
	description VARCHAR NOT NULL default ""
);
CREATE UNIQUE INDEX index_foods_on_name ON foods(name);


CREATE TABLE questionaire_people (
	questionaire_id INTEGER NOT NULL,
	person_id INTEGER NOT NULL,
        UNIQUE(questionaire_id,person_id)
);

CREATE INDEX index_questionaire_people_on_questionaire ON questionaire_people(questionaire_id);

CREATE TABLE food_nutrients (
	food_id INTEGER NOT NULL,
	nutrient_id INTEGER NOT NULL,
	amount float,
        UNIQUE(food_id,nutrient_id)
);
CREATE INDEX index_food_nutrients_on_food ON food_nutrients(food_id);

CREATE TABLE person_meals (
	person_id INTEGER NOT NULL,
	meal_id INTEGER NOT NULL,
	frequency float,
        UNIQUE(person_id,meal_id)
);
CREATE INDEX index_person_meals_on_person ON person_meals(person_id);

CREATE TABLE person_foods (
	person_id INTEGER NOT NULL,
	food_id INTEGER NOT NULL,
	portion INTEGER NOT NULL,
	frequency float NOT NULL
);
CREATE INDEX index_person_foods_on_person ON person_foods(person_id);


CREATE TABLE meal_foods (
	meal_id INTEGER NOT NULL,
	food_id INTEGER NOT NULL,
	portion INTEGER NOT NULL,
        UNIQUE(meal_id,food_id)
);
CREATE INDEX index_meal_foods_on_meal ON meal_foods(meal_id);

