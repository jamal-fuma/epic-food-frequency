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

CREATE TABLE portions (
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
	amount REAL NOT NULL default 0,
	UNIQUE(food_id,nutrient_id)
);
CREATE INDEX index_food_nutrients_on_food ON food_nutrients(food_id);

CREATE TABLE meal_foods (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	meal_id INTEGER NOT NULL,
	food_id INTEGER default NULL,
	amount  REAL   NOT NULL,
	modifier INTEGER NOT NULL default 0
);
CREATE INDEX index_meal_foods_on_meal ON meal_foods(meal_id);
CREATE INDEX index_meal_foods_on_meal_and_modifier ON meal_foods(meal_id,modifier);

CREATE TABLE cereals (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	food_id     INTEGER UNIQUE NOT NULL,
	weight_id   INTEGER NOT NULL
);

CREATE TABLE milks (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	code INTEGER UNIQUE NOT NULL,
	food_id     INTEGER NOT NULL
);

CREATE TABLE person_fats (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	person_id   INTEGER NOT NULL,
	food_id     INTEGER NOT NULL,
	modifier INTEGER NOT NULL default 0
);
CREATE INDEX index_person_fats_on_person_and_modifier_and_food_id ON person_fats(person_id,food_id,modifier);
CREATE INDEX index_person_fats_on_person_and_modifier ON person_fats(person_id,modifier);

CREATE TABLE person_weights (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	person_id   INTEGER NOT NULL,
	weight_id   INTEGER NOT NULL,
	modifier INTEGER NOT NULL default 0
);
CREATE INDEX index_person_weights_on_person_and_modifier_and_weight ON person_weights(person_id,weight_id,modifier);

CREATE TABLE person_cereals (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	person_id   INTEGER NOT NULL,
	food_id     INTEGER NOT NULL,
	weight_id   INTEGER NOT NULL,
	modifier    INTEGER NOT NULL default 3
);
CREATE INDEX index_person_cereals_on_person_and_modifier ON person_cereals(person_id,modifier);
CREATE INDEX index_person_cereals_on_person_food_and_modifier_and_weight ON person_cereals(person_id,food_id,weight_id,modifier);

CREATE TABLE person_meals (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	person_id       INTEGER NOT NULL,
	meal_id         INTEGER NOT NULL,
	frequency_id    INTEGER NOT NULL
);

CREATE INDEX index_person_meals_on_person_meal_and_frequency ON person_meals(person_id,meal_id,frequency_id);
CREATE INDEX index_person_meals_on_meal_and_frequency ON person_meals(meal_id,frequency_id);

CREATE TABLE person_foods (
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
	person_id INTEGER NOT NULL,
	food_id INTEGER NOT NULL,
	frequency_id    INTEGER NOT NULL,
	portion_id INTEGER NOT NULL
);
CREATE INDEX index_person_foods_on_person ON person_foods(person_id);
CREATE INDEX index_person_foods_on_frequency_id_and_portion_id ON person_foods(frequency_id,portion_id);


CREATE VIEW person_fat_totals_vw AS SELECT
    person_id,
    count(person_id) AS entries,
    modifier
FROM
    person_fats
GROUP BY
    person_id,
    modifier ;

CREATE VIEW person_cereal_totals_vw AS SELECT
    person_id,
    count(person_id) AS entries,
    modifier
FROM
    person_cereals
GROUP BY
    person_id,
    modifier ;

CREATE VIEW person_milk_totals_vw AS SELECT
    person_id,
    count(person_id) AS entries
FROM
    person_foods
GROUP BY
    person_id;


CREATE VIEW person_fat_foods_vw AS SELECT
    person_meals.person_id,
    person_meals.meal_id,
    person_fats.food_id,
    ( (frequencies.amount) * (meal_foods.amount / person_fat_totals_vw.entries)) AS amount
FROM
    person_meals,
    meal_foods,
    person_fats,
    person_fat_totals_vw,
    frequencies
WHERE
    person_fats.person_id               = person_fat_totals_vw.person_id
    AND person_fats.modifier            = person_fat_totals_vw.modifier
    AND person_meals.person_id          = person_fats.person_id
    AND person_meals.meal_id            = meal_foods.meal_id
    AND person_meals.frequency_id       = frequencies.id
    AND person_fats.modifier            = meal_foods.modifier
ORDER BY
    person_fats.person_id;


CREATE VIEW person_cereal_foods AS SELECT
    person_cereals.person_id,
    meal_foods.meal_id,
    person_cereals.food_id,
    ( (frequencies.amount) * ((meal_foods.amount / person_cereal_totals_vw.entries) / weights.amount) ) AS amount
FROM
    person_meals,
    person_cereals,
    person_cereal_totals_vw,
    frequencies,
    weights,
    meal_foods
WHERE
    person_cereals.person_id             = person_meals.person_id
    AND person_cereals.modifier          = meal_foods.modifier
    AND person_cereals.weight_id         = weights.id
    AND person_meals.frequency_id        = frequencies.id
    AND meal_foods.meal_id               = person_meals.meal_id
    AND person_cereals.person_id         = person_cereal_totals_vw.person_id
    AND person_cereal_totals_vw.modifier = person_cereals.modifier
ORDER BY
    person_meals.person_id;


CREATE VIEW person_meat_foods_vw AS SELECT
    person_weights.person_id,
    meal_foods.meal_id,
    meal_foods.food_id,
    ((meal_foods.amount * weights.amount) * frequencies.amount) AS amount
FROM
    person_weights,
    person_meals,
    frequencies,
    weights,
    meal_foods
WHERE
    person_weights.person_id        = person_meals.person_id
    AND person_weights.weight_id    = weights.id
    AND meal_foods.modifier         = person_weights.modifier
    AND person_meals.frequency_id   = frequencies.id
    AND meal_foods.meal_id          = person_meals.meal_id
ORDER BY
    person_weights.person_id;

/* dirty hack to allow combining with meal reporting  */
CREATE VIEW person_milk_foods_vw AS SELECT
    person_foods.person_id,
    0 AS meal_id,
    person_foods.food_id,
    ((portions.amount * frequencies.amount) / person_milk_totals_vw.entries) AS amount
FROM
    person_foods,
    frequencies,
    portions,
    person_milk_totals_vw
WHERE
    person_foods.frequency_id   = frequencies.id
    AND person_foods.portion_id = portions.id
    AND person_foods.person_id  = person_milk_totals_vw.person_id
ORDER BY
    person_foods.person_id;

CREATE VIEW person_simple_foods_vw AS SELECT
    person_meals.person_id,
    meal_foods.meal_id,
    meal_foods.food_id,
    (meal_foods.amount * frequencies.amount) AS amount
FROM
    person_meals,
    frequencies,
    meal_foods
WHERE
    meal_foods.modifier = 0
    AND person_meals.frequency_id = frequencies.id
    AND meal_foods.meal_id = person_meals.meal_id
ORDER BY
    person_meals.person_id;

CREATE VIEW person_food_vw AS SELECT
    person_id,meal_id,food_id,amount FROM person_fat_foods_vw
WHERE
    amount > 0
UNION ALL SELECT
    person_id,meal_id,food_id,amount FROM person_cereal_foods
WHERE
    amount > 0
UNION ALL SELECT
    person_id,meal_id,food_id,amount FROM person_meat_foods_vw
WHERE
    amount > 0
UNION ALL SELECT
    person_id,meal_id,food_id,amount FROM person_simple_foods_vw
WHERE
    amount > 0
UNION ALL SELECT
    person_id,meal_id,food_id,amount FROM person_milk_foods_vw
WHERE
    amount > 0
ORDER BY person_id,meal_id,food_id;

CREATE TABLE meal_nutrients_tmp (
	meal_id INTEGER NOT NULL,
	nutrient_code INTEGER NOT NULL,
	quantity  REAL   NOT NULL
);
CREATE INDEX index_meal_nutrients_tmp_on_meal_and_nutrient ON meal_nutrients_tmp(meal_id,nutrient_code);

CREATE VIEW meal_nutrients_vw AS SELECT
    meal_id,nutrient_code,sum(quantity) as quantity
FROM
    meal_nutrients_tmp
GROUP BY
    meal_id,nutrient_code;

CREATE VIEW nutrients_vw AS SELECT
    nutrient_code, sum(quantity) as quantity
FROM
    meal_nutrients_vw
GROUP BY
    nutrient_code;


CREATE TRIGGER trigger_questionaire_clean AFTER DELETE ON questionaires
BEGIN
    DELETE FROM questionaire_people WHERE questionaire_id = OLD.id;
END;

CREATE TRIGGER trigger_questionaire_people_clean AFTER DELETE ON questionaire_people
BEGIN
    DELETE FROM people              WHERE id        = OLD.person_id;
    DELETE FROM questionaire_people WHERE person_id = OLD.person_id;
    DELETE FROM person_meals        WHERE person_id = OLD.person_id;
    DELETE FROM person_fats         WHERE person_id = OLD.person_id;
    DELETE FROM person_foods        WHERE person_id = OLD.person_id;
    DELETE FROM person_cereals      WHERE person_id = OLD.person_id;
    DELETE FROM person_weights      WHERE person_id = OLD.person_id;
    DELETE FROM meal_nutrients_tmp ;
END;
