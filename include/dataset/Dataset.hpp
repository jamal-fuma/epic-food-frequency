#ifndef EPIC_DATASET_HPP
#define EPIC_DATASET_HPP

#include <string>
#include <map>
#include <set>
#include <sstream>
#include <ostream>


namespace Epic
{
     namespace Dataset
    {
        template <class Item>
        struct SetValuesPolicy
        {
            typedef std::set<Item> Container;
            static void insert( Container & c,
                                const Item item) {
                c.insert(item);
            }
            static void remove( Container & c,
                                const Item item) {
                c.erase(item);
            }
        };

        // forward declaration for child version
        // of relation
        template <  class Parent,
                    class Child,
                    class Relation >
        class RelationChild ;

        // parent version of relation
        template <  class Parent,
                    class Child,
                    class Relation = void,
                    class Policy = SetValuesPolicy<Child*> >
        class RelationParent
        {
            typedef typename Policy::Container Container;
            typedef typename Container::const_iterator const_iterator_t;
            typedef typename Container::iterator iterator_t;

            friend class RelationChild<Parent,Child,Relation>;
        public:

            // iterators
            const_iterator_t
            begin()
            {
                return m_children.begin();
            }
            const_iterator_t
            end()
            {
                return m_children.end();
            }

            size_t
            size()
            {
                return m_children.size();
            }

            void
            attach(Child *owned)
            {
                Policy::insert(m_children,owned);
            }

            void
            detach(Child *owned)
            {
                Policy::remove(m_children,owned);
            }


            ~RelationParent()
            {
                Container tmp;
                // move into a temp prior to clean up
                swap(m_children,tmp);

                const_iterator_t tmp_end = tmp.end();
                for(iterator_t it = tmp.begin();
                        it != tmp_end ; ++it)
                {
                    delete *it;
                }
            }
        private:
            Container m_children;

        };

        // definition for child version
        // of relation
        template <  class Parent,
                    class Child,
                    class Relation = void>
        class RelationChild
        {
            // parent version of relation
            typedef RelationParent <
                Parent,
                Child,
                Relation> Relationship;
        public:
            explicit
            RelationChild(Parent *parent) : m_parent(parent)
            {
                m_parent->Relationship::attach(child());
            }

            void
            reset()
            {
                if(m_parent)
                {
                    m_parent->Relationship::detach(child());
                    m_parent = NULL;
                }
            }
            ~RelationChild()
            {
                reset();
            }

        private:
            Child* child()
            {
                return static_cast<Child*>(this);
            }
            Parent *m_parent;
        };

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
