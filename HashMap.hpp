#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#include "SimpleHashFunctor.hpp"
#include "EqualsFunctor.hpp"

// standard libraries
#include <stdlib.h>
#include <string.h>

/**
 * @brief a fast hashmap implemention
 */
template<class Key, class Value, class Hash_Functor = SimpleHashFunctor, class Equals_Functor = EqualsFunctor>
class HashMap
{
public:

    struct HashMapNode
    {
	HashMapNode* next_;
	unsigned long bucket_no_;
	Key key_;
	Value value_;
    };

    //iterator
    class iterator
    {
    public:
	iterator(HashMap* map, HashMapNode* node): map_(map), node_(node) {}
	iterator(const iterator& iter): map_(iter.map_), node_(iter.node_) {}

	iterator& operator=(const iterator& iter)
	{
	    if (iter == *this)
		return *this;
	    map_ = iter.map_;
	    node_ = iter.node_;
	}

	bool operator==(const iterator& iter) const
	{
	    return (iter.node_ == node_);
	}

	bool operator!=(const iterator& iter) const
	{
	    return (iter.node_ != node_);
	}

	HashMapNode* operator->()
	{
	    return node_;
	}

	HashMapNode& operator*()
	{
	    return *node_;
	}

	iterator& operator++() // pre
	{
	    HashMapNode* next = node_->next_;
	    if (!next)
	    {
		for (unsigned long i = node_->bucket_no_ + 1; i < map_->bucket_count_; ++i)
		{
		    if (next = map_->buckets_[i])
			break;
		}
	    }

	    node_ = next;
	    return *this;
	}

	iterator operator++(int) //post
	{
	    HashMapNode* next = node_->next_;
	    if (!next)
	    {
		for (unsigned long i = node_->bucket_no_ + 1; i < map_->bucket_count_; ++i)
		{
		    if (next = map_->buckets_[i])
			break;
		}
	    }

	    node_ = next;
	    return iterator(map_, node_);
	}


    private:
	friend class HashMap;
	HashMap* map_;
	HashMapNode* node_;
    };


    HashMap(unsigned int bucket_count = 0): buckets_(0), size_(0), bucket_count_(bucket_count), auto_resize_ratio_(2)
    {
	if (bucket_count_ < 1)
	    bucket_count_ = 67;
    }
    iterator begin();
    iterator end() { return iterator(this, 0); }
    void resize(unsigned long bucket_count);
    HashMapNode** createBuckets(unsigned long bucket_count);
    HashMapNode* createNode(unsigned long bucket_count, HashMapNode* next);
    
    iterator findNode(const Key& key);
    bool deleteNode(const Key& key);
    bool deleteNode(iterator iter);
    bool deleteNode(HashMapNode* node);
    void destroy(HashMapNode* node);

    void clear()
    {
	if (buckets_)
	{
	    for (unsigned long i = 0; i < bucket_count_; ++i)
	    {
		HashMapNode* node = buckets_[i];
		while(node)
		{
		    node->key_.~Key();
		    node->value_.~Value();
		    HashMapNode* next_node = node->next_;
		    free(node);
		    node = next_node;
		}
		buckets_[i] = 0;
	    }
	    size_ = 0;
	}
    }

    void statistics() { cout << "Total Bucket Count [" << bucket_count_ << "]. Total size [" << size_ << "]." << endl; }

    Value& operator[](const Key& key)
    {
	unsigned long bucket_no;
	HashMapNode* node;
	if (!(node = findNode(key, bucket_no)))
	{
	    if (!buckets_)
	    {
		buckets_ = createBuckets(bucket_count_);
	    }
	    node = createNode(bucket_no, buckets_[bucket_no]);
	    buckets_[bucket_no] = node;
	    ::new(&node->key_) Key(key);
	    ::new(&node->value_) Value();
	    if (auto_resize_ratio_ && (bucket_count_ * auto_resize_ratio_) < size_)
		resize(bucket_count_ * auto_resize_ratio_); // resize the hash map
	}

	return node->value_;
    }

    ~HashMap() { clear(); }
private:

    HashMapNode* findNode(const Key& key, unsigned long& bucket_no)
    {
	bucket_no = getHash_(key) % bucket_count_;

	if (!size_)
	    return 0;

	HashMapNode* node = buckets_[bucket_no];
	while(node && !isEqual_(node->key_, key))
	{
	    node = node->next_;
	}	
	return node;
    }


    HashMapNode** buckets_;
    unsigned long size_;
    unsigned long bucket_count_;
    unsigned long auto_resize_ratio_;

    Hash_Functor getHash_;
    Equals_Functor isEqual_;

    // copy constructor and assignment operators not used
    HashMap(const HashMap&) {};
    HashMap& operator=(const HashMap&) {};
};

#include "HashMap.inl"

#endif //_HASHMAP_HPP_
