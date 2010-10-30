#ifndef EPIC_DATASET_HPP
#define EPIC_DATASET_HPP

#include "../include/Epic.hpp"

namespace Epic
{
    namespace Dataset
    {
        struct Preceeding {};
        struct Following {};

        class Meal;
        class Ingredient;

        typedef RelationParent<Meal,Ingredient,Preceeding> PreceedingMeal;
        typedef RelationParent<Meal,Ingredient,Following>  FollowingMeal;
 
        typedef RelationChild<Meal,Ingredient,Preceeding> PreceedingIngredient;
        typedef RelationChild<Meal,Ingredient,Following>  FollowingIngredient;

        class Meal :
            public PreceedingMeal,
            public FollowingMeal
        {
        public:
            typedef PreceedingMeal Preceeding;
            typedef FollowingMeal  Following;
            Meal() {}
        };
        
        class Ingredient :
            public PreceedingIngredient,
            public FollowingIngredient
        {
        public:
            typedef PreceedingIngredient Preceeding;
            typedef FollowingIngredient  Following;
            
            Ingredient(Meal *meal_prior, Meal *meal_after)
                : Preceeding(meal_prior), 
                  Following(meal_after)
            {
            
            }
        };

     }
}


#endif /* EPIC_DATASET_HPP */
