#include <string>
#include "Index.h"
#include "AvlTreeIndex.h"
#include "List.h"
#include "exception.h"
#include "TransactionImpl.h"
#include "GraphImpl.h"
#include "IndexString.h"

using namespace Jarvis;

void Index::init(PropertyType ptype)
{
    unsigned size = 0;

    switch(ptype) {
        case t_integer:
            static_cast<LongValueIndex *>(this)->init();
            size = sizeof(LongValueIndex);
            break;
        case t_float:
            static_cast<FloatValueIndex *>(this)->init();
            size = sizeof(FloatValueIndex);
            break;
        case t_boolean:
            static_cast<BoolValueIndex *>(this)->init();
            size = sizeof(BoolValueIndex);
            break;
        case t_string:
            static_cast<StringValueIndex *>(this)->init();
            size = sizeof(StringValueIndex);
            break;
        case t_time:
        case t_novalue:
            throw Exception(not_implemented);
        case t_blob:
        default:
            throw Exception(property_type);
    }
    _ptype = ptype;

    // The AvlTreeIndex init() is specifically kept without any TX
    // flush right now since it is always used from within the parent
    // class Index and we can just flush the entire data struct from
    // here.
    TransactionImpl::flush_range(this, size);
}

void Index::add(const Property &p, Node *n, GraphImpl *db)
{
    if (_ptype != p.type())
        throw Exception(property_type);

    Allocator &allocator = db->allocator();

    // TODO: Tree is unnecessary and Node* needs better arrangement for
    // quick search and remove operations
    List<Node*> *dest = NULL;

    switch(_ptype) {
        case t_integer:
            dest = static_cast<LongValueIndex *>(this)->add(p.int_value(),
                                                            allocator);
            break;
        case t_float:
            dest = static_cast<FloatValueIndex *>(this)->add(p.float_value(),
                                                             allocator);
            break;
        case t_boolean:
            dest = static_cast<BoolValueIndex *>(this)->add(p.bool_value(),
                                                            allocator);
            break;
        case t_string:
            {
                TransientIndexString istr(p.string_value(), db->locale());
                dest = static_cast<StringValueIndex *>(this)->add(istr, allocator);
            }
            break;
        case t_time:
        case t_novalue:
            throw Exception(not_implemented);
        case t_blob:
        default:
            throw Exception(property_type);
    }
    // dest will never be null since it gets allocated at the add time.
    // The List->init() function does not do a transaction flush but
    // we are going to add an element right after before the transaction
    // gets over and that should do the right logging of the very same
    // elements that get modified in init().
    if (dest->num_elems() == 0)
        dest->init();
    dest->add(n, allocator);
}

void Index::remove(const Property &p, Node *n, GraphImpl *db)
{
    if (_ptype != p.type())
        throw Exception(property_type);

    Allocator &allocator = db->allocator();

    List<Node*> *dest;
    switch(_ptype) {
        case t_integer:
            {
                LongValueIndex *prop_idx = static_cast<LongValueIndex *>(this);
                dest = prop_idx->find(p.int_value());
                if (dest) {
                    dest->remove(n, allocator);
                    // TODO: Re-traversal of tree.
                    if (dest->num_elems() == 0)
                        prop_idx->remove(p.int_value(), allocator);
                }
            }
            break;
        case t_float:
            {
                FloatValueIndex *prop_idx = static_cast<FloatValueIndex *>(this);
                dest = prop_idx->find(p.float_value());
                if (dest) {
                    dest->remove(n, allocator);
                    // TODO: Re-traversal of tree.
                    if (dest->num_elems() == 0)
                        prop_idx->remove(p.float_value(), allocator);
                }
            }
            break;
        case t_boolean:
            {
                BoolValueIndex *prop_idx = static_cast<BoolValueIndex *>(this);
                dest = prop_idx->find(p.bool_value());
                if (dest) {
                    dest->remove(n, allocator);
                    // TODO: Re-traversal of tree.
                    if (dest->num_elems() == 0)
                        prop_idx->remove(p.float_value(), allocator);
                }
            }
            break;
        case t_string:
            {
                TransientIndexString istr(p.string_value(), db->locale());
                StringValueIndex *prop_idx = static_cast<StringValueIndex *>(this);
                dest = prop_idx->find(istr);
                if (dest) {
                    dest->remove(n, allocator);
                    // TODO: Re-traversal of tree.
                    if (dest->num_elems() == 0)
                        prop_idx->remove(istr, allocator);
                }
            }
            break;
        case t_time:
        case t_novalue:
            throw Exception(not_implemented);
        case t_blob:
        default:
            throw Exception(property_type);
    }
}

namespace Jarvis {
    class IndexEq_NodeIteratorImpl : public NodeIteratorImplIntf {
        const List<Node *>::ListType *_pos;
    public:
        IndexEq_NodeIteratorImpl(List<Node *> *l)
            : _pos(l->begin())
        { }
        Node &operator*() const { return *_pos->value; }
        Node *operator->() const { return _pos->value; }
        Node &operator*() { return *_pos->value; }
        Node *operator->() { return _pos->value; }
        operator bool() const { return _pos != NULL; }
        bool next()
        {
            _pos = _pos->next; 
            return _pos != NULL;
        }
    };
}

NodeIterator Index::get_nodes(const PropertyPredicate &pp, std::locale *loc)
{
    // Since we will not have range support yet, only the equal relation
    // is valid
    if (pp.op != PropertyPredicate::eq)
        throw Exception(not_implemented);

    // TODO: Only handling eq case right now and the check has
    // already been done when this is called. Use value v1 from pp.
    const Property &p = pp.v1;
    List<Node *> *list = NULL;

    if (_ptype != p.type())
        throw Exception(property_type);
    switch(_ptype) {
        case t_integer:
            {
                LongValueIndex *prop_idx = static_cast<LongValueIndex *>(this);
                list = prop_idx->find(p.int_value());
            }
            break;
        case t_float:
            {
                FloatValueIndex *prop_idx = static_cast<FloatValueIndex *>(this);
                list = prop_idx->find(p.float_value());
            }
            break;
        case t_boolean:
            {
                BoolValueIndex *prop_idx = static_cast<BoolValueIndex *>(this);
                list = prop_idx->find(p.bool_value());
            }
            break;
        case t_string:
            {
                TransientIndexString istr(p.string_value(), *loc);
                StringValueIndex *prop_idx = static_cast<StringValueIndex *>(this);
                list = prop_idx->find(istr);
            }
            break;
        case t_time:
        case t_novalue:
            throw Exception(not_implemented);
        case t_blob:
        default:
            throw Exception(property_type);
    }
    if (list == NULL)
        return NodeIterator(NULL);
    else
        return NodeIterator(new IndexEq_NodeIteratorImpl(list));
}
