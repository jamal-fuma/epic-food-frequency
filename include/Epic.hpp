#ifndef EPIC_HPP
#define EPIC_HPP

#define GPL_LICENSE_TEXT ( \
"\nThis is free software: you can redistribute it and/or modify" \
"\nit under the terms of the GNU General Public License as published by" \
"\nthe Free Software Foundation, either version 3 of the License, or" \
"\n(at your option) any later version." \
"\n" \
"\nThis software is distributed in the hope that it will be useful," \
"\nbut WITHOUT ANY WARRANTY; without even the implied warranty of" \
"\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" \
"\nGNU General Public License for more details." \
"\n" \
"\nYou should have received a copy of the GNU General Public License" \
"\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.")


#include <libUtility.h>

#include <string>
#include <map>
#include <set>
#include <sstream>
#include <ostream>


namespace Epic
{
    namespace Server
    {
    
    }

    namespace Client
    {
       
    }
    
    class IFile
    {
        public:
            virtual operator FILE * () const = 0;
            virtual operator std::string () const = 0;
        protected:
            virtual void open(FILE * fp,  const std::string & mode="w") = 0;
            virtual void open(const std::string & mode="w") = 0;
            virtual void close() = 0;
    };
         
    class File : public IFile
    {
        public:
            File(const std::string & filename, const std::string & mode="rb") throw(std::string) : 
                m_fp(NULL),
                m_filename(filename)
            {
                open(mode);
            }
        
            File(FILE * &fp, const std::string &filename, const std::string & mode="w")  throw(std::string) :
                m_fp(fp),
                m_filename(filename) 
            {
                open(fp,mode);
            }

            virtual ~File()
            {
                close();
            }

            
            // decay into file pointer
            inline operator FILE * () const
            {
                return m_fp;
            }
            
            // decay into filename
            inline operator std::string () const
            {
                return m_filename;
            }
        
        protected:
     
            void
            open(const std::string & mode="rb")
            {
                m_fp = ::fopen(m_filename.c_str(),mode.c_str());
                if(!m_fp)
                {
                    std::string err = "Unable to open input file : ";
                    err += m_filename;
                    throw err;
                }
            }
            
            void
            open(FILE * fp,  const std::string & mode="w")
            {
                if(!m_fp)
                {
                    std::string err = "Unable to open file : ";
                    err += m_filename;
                    throw err;
                }
                m_fp = ::freopen(m_filename.c_str(),mode.c_str(),m_fp);
                if(!m_fp)
                {
                    std::string err = "Unable to open file : ";
                    err += m_filename;
                    throw err;
                }
            }
            
            void
            close()
            {
                if(m_fp)
                {
                    ::fclose(m_fp);
                    m_fp = NULL;
                }
            }
    private:
        File();
        File(const File & rhs);

        FILE *m_fp;
        std::string m_filename;
    };  
 
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
    }
}

#endif /* EPIC_HPP */
