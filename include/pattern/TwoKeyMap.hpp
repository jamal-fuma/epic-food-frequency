#ifndef EPIC_TWO_KEY_MAP_HPP
#define EPIC_TWO_KEY_MAP_HPP

#include <map>
#include <stdexcept> // std::out_of_range
#include <sstream>   // ostringstream

namespace Epic
{
    namespace Pattern
    {
        template <typename Key1, typename Value, typename Key2=Key1> 
        class TwoKeyMap
        {
            typedef std::map< Key1, std::pair<Key2,Value> > FrontMap_t;
            typedef typename FrontMap_t::const_iterator     Front_const_iterator_t;
            typedef typename FrontMap_t::iterator           Front_iterator_t;

            typedef std::map< Key2, Key1> BackMap_t;
            typedef typename BackMap_t::const_iterator      Back_const_iterator_t;
            typedef typename BackMap_t::iterator            Back_iterator_t;

            BackMap_t  m_back_map;
            FrontMap_t m_front_map;


            Front_const_iterator_t
            find_front(const Key1 & key) const throw(std::out_of_range)
            {
                Front_const_iterator_t ret;
                ret = m_front_map.find(key);
                if(ret == m_front_map.end())
                {
                    std::ostringstream s;
                    s << "no value found for primary key [" << key << "]" << std::endl;
                    throw std::out_of_range(s.str());
                }

                return ret;
            }

            Back_const_iterator_t
            find_back(const Key2 & key) const throw(std::out_of_range)
            {
                Back_const_iterator_t ret;
                ret = m_back_map.find(key);
                if(ret == m_back_map.end())
                {
                    std::ostringstream s;
                    s << "no value found for secondary key [" << key << "]" << std::endl;
                    throw std::out_of_range(s.str());
                }
                return ret;
            }

            void
            erase_front(const Key1 & key)
            {
                Front_iterator_t it = m_front_map.find(key);
                if(it == m_front_map.end())
                    return; 

                m_back_map.erase(it->second.first);
                m_front_map.erase(it);
            }

            void
            erase_back(const Key2 & key)
            {
                Back_iterator_t it = m_back_map.find(key);
                if(it == m_back_map.end())
                    return; 

                m_front_map.erase(it->second);
                m_back_map.erase(it);
            }

            public:
            void
            clear()
            {
                m_front_map.clear();
                m_back_map.clear();
            }

            void
            insert(
                const Key1 & primary,
                const Key2 & secondary,
                const Value & value
            ) throw (std::out_of_range)
            {
                erase_front(primary);    
                erase_back(secondary);
                m_back_map[secondary] = primary;
                m_front_map[primary]  = std::make_pair(secondary,value);
            }

            const Value
            primary(const Key1 & key) const throw (std::out_of_range)
            {
                return find_front(key)->second.second;
            }

            const Value
            secondary(const Key2 & key) const throw (std::out_of_range)
            {
                return primary(find_back(key)->second);
            }

        };
    } // Epic::Pattern
} // Epic

#endif /*ndef EPIC_TWO_KEY_MAP_HPP */
