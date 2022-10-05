/*
 * COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
 *
 * 
 * For the purposes of this copyright and license, "Contributing Authors"
 * is defined as the following set of individuals:
 *
 *    Carlos Augusto Dietrich (cadietrich@gmail.com)
 *
 * This library is supplied "AS IS".  The Contributing Authors disclaim 
 * all warranties, expressed or implied, including, without limitation, 
 * the warranties of merchantability and of fitness for any purpose. 
 * The Contributing Authors assume no liability for direct, indirect, 
 * incidental, special, exemplary, or consequential damages, which may 
 * result from the use of the this library, even if advised of the 
 * possibility of such damage.
 *
 * Permission is hereby granted to use, copy, modify, and distribute this
 * source code, or portions hereof, for any purpose, without fee, subject
 * to the following restrictions:
 *
 * 1. The origin of this source code must not be misrepresented.
 *
 * 2. Altered versions must be plainly marked as such and must not be 
 *    misrepresented as being the original source.
 *
 * 3. This Copyright notice may not be removed or altered from any source 
 *    or altered source distribution.
 *
 * The Contributing Authors specifically permit, without fee, and 
 * encourage the use of this source code as a component in commercial 
 * products. If you use this source code in a product, acknowledgment 
 * is not required but would be appreciated.
 *
 * 
 * "Software is a process, it's never finished, it's always evolving. 
 * That's its nature. We know our software sucks. But it's shipping! 
 * Next time we'll do better, but even then it will be shitty. 
 * The only software that's perfect is one you're dreaming about. 
 * Real software crashes, loses data, is hard to learn and hard to use. 
 * But it's a process. We'll make it less shitty. Just watch!"
 */

#if !defined(OBJECT_INCLUDED)
#define OBJECT_INCLUDED

#include <vector>

#include <boost/shared_ptr.hpp>

class CObject
{
public:
    enum {
        INVALID_ID = -1
    };

    enum {
        INVALID_TYPE = -1
    };

    typedef int IdType;
	typedef std::vector<boost::shared_ptr<CObject> > ChildArrayType;

	CObject();
	CObject(const CObject& object);

	virtual ~CObject();

	void operator=(const CObject& object);

	void SetParent(boost::shared_ptr<CObject> parent);
	boost::shared_ptr<CObject> GetParent() const;

	void AddChild(boost::shared_ptr<CObject> child);
	void AddChild(ChildArrayType::iterator position, boost::shared_ptr<CObject> child);
	void RemoveChild(boost::shared_ptr<CObject> child);
	ChildArrayType GetChildren() const;

    IdType GetId() const;
    void SetId(IdType id);

	virtual int GetObjectType() const;

protected:
    static IdType GetUniqueId();

private:
    static IdType m_idCount;

	boost::shared_ptr<CObject> m_parent;

	ChildArrayType m_childArray;

    IdType m_id;
};

#endif //#if !defined(OBJECT_INCLUDED)

