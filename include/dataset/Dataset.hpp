/* This file is part of the libEpic library */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EPIC_DATASET_HPP
#define EPIC_DATASET_HPP

#include <string>
#include <map>
#include <set>
#include <sstream>
#include <ostream>
#include <tr1/memory>


#include "dao/Questionaire.hpp"
#include "dao/Person.hpp"

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
                    class Child>
        class RelationChild ;

        // parent version of relation
        template <  class Parent,
                    class Child,
                    class Policy = SetValuesPolicy<Child*> >
        class RelationParent
        {
            typedef typename Policy::Container Container;
            typedef typename Container::iterator iterator_t;

            friend class RelationChild<Parent,Child>;
        public:
            typedef typename Container::const_iterator const_iterator_t;

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
                    class Child>

        class RelationChild
        {
            // parent version of relation
            typedef RelationParent <
                Parent,
                Child> Relationship;
        public:
            explicit
            RelationChild(Parent *parent) : m_parent(parent)
            {
                if(m_parent)
                {
                    m_parent->Relationship::attach(child());
                }
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

        class Questionaire;
        class Person;

        class Questionaire :
            public RelationParent<Questionaire,Person>,
            public Epic::DAO::Questionaire
        {
        public:
            Questionaire() {}

        };

        class Person :
            public RelationChild<Questionaire,Person>,
            public Epic::DAO::Person
        {
        public:

            Person(Questionaire *questionaire)
                : RelationChild<Questionaire,Person>(questionaire)
            {

            }
        };

     }
}


#endif /* EPIC_DATASET_HPP */
