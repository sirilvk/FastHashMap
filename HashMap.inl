template<class Key, class Value, class Hash_Functor, class Equals_Functor>
typename HashMap<Key, Value, Hash_Functor, Equals_Functor>::iterator 
HashMap<Key, Value, Hash_Functor, Equals_Functor>::begin()
{
    if (buckets_)
    {
        for (unsigned long i = 0; i < bucket_count_; ++i)
	    if (buckets_[i])
	       return iterator(this, buckets_[i]);
    }
    return iterator(this, 0);
}

template<class Key, class Value, class Hash_Functor, class Equals_Functor>
void HashMap<Key, Value, Hash_Functor, Equals_Functor>::resize(unsigned long bucket_count)
{
    if (bucket_count == bucket_count_)
	return;

//    cout << "Resizing the buckets from [" << bucket_count_ << "] to [" << bucket_count << "] for a total size of [" << size_ << "]" << endl;
    // create new buckets
    HashMapNode** buckets = createBuckets(bucket_count);

    if (size_)
    {
	// for nodes in the existing bucket we need to reallocate them to the new bucket and also
	// correct the link to the author name tries and the category tries.
	for (unsigned long i = 0; i < bucket_count_; ++i)
	{
	    HashMapNode* node = buckets_[i];
	    while(node)
	    {
		HashMapNode* node_next = node->next_;
		node->bucket_no_ = getHash_(node->key_) % bucket_count; // get the new bucket position
		node->next_ = buckets[node->bucket_no_];
		buckets[node->bucket_no_] = node;
		node = node_next;
	    }
	}

	free(buckets_);
	buckets_ = buckets;
        bucket_count_ = bucket_count;
	return;
    }

    if (buckets_)
    {
	free(buckets_);
	buckets_ = buckets;
    }

    bucket_count_ = bucket_count;
}

template<class Key, class Value, class Hash_Functor, class Equals_Functor>
typename HashMap<Key, Value, Hash_Functor, Equals_Functor>::HashMapNode**
HashMap<Key, Value, Hash_Functor, Equals_Functor>::createBuckets(unsigned long bucket_count)
{
    HashMapNode** buckets = (HashMapNode**)(malloc(bucket_count * sizeof(HashMapNode*)));
    memset(buckets, 0, bucket_count * sizeof(HashMapNode*));
    return buckets;
}

template<class Key, class Value, class Hash_Functor, class Equals_Functor>
typename HashMap<Key, Value, Hash_Functor, Equals_Functor>::HashMapNode*
HashMap<Key, Value, Hash_Functor, Equals_Functor>::createNode(unsigned long bucket_no, HashMapNode* next)
{
    ++size_;
    HashMapNode* node = (HashMapNode*) malloc(sizeof(HashMapNode));
    node->bucket_no_ = bucket_no;
    node->next_ = next;
    return node;
}

template<class Key, class Value, class Hash_Functor, class Equals_Functor>
typename HashMap<Key, Value, Hash_Functor, Equals_Functor>::iterator 
HashMap<Key, Value, Hash_Functor, Equals_Functor>::findNode(const Key& key)
{
    if (buckets_)
    {
        unsigned long bucket_no = getHash_(key) % bucket_count_;
    	HashMapNode* node = buckets_[bucket_no];
    	while(node && !isEqual_(node->key_, key))
    	{
	    node = node->next_;
    	}
	return iterator(this, node);
    }

    return iterator(this, 0);
}

template<class Key, class Value, class Hash_Functor, class Equals_Functor>
bool HashMap<Key, Value, Hash_Functor, Equals_Functor>::deleteNode(const Key& key)
{
    unsigned long bucket_no = getHash_(key) % bucket_count_;
    HashMapNode** previous = &buckets_[bucket_no];
    HashMapNode* node = *previous;
    while(node)
    {
	if (isEqual_(node->key_, key))
	{
	    // set the previous node to the next
	    *previous = node->next_;
	    destroy(node);
	    return true;
	}
	previous = &node->next_;
	node = node->next_;
    }

    return false;
}

template<class Key, class Value, class Hash_Functor, class Equals_Functor>
bool HashMap<Key, Value, Hash_Functor, Equals_Functor>::deleteNode(iterator iter)
{
    if (iter != end())
	return deleteNode(iter->key_);
}

template<class Key, class Value, class Hash_Functor, class Equals_Functor>
bool HashMap<Key, Value, Hash_Functor, Equals_Functor>::deleteNode(HashMapNode* node)
{
    if (node)
	return deleteNode(node->key_);
}

template<class Key, class Value, class Hash_Functor, class Equals_Functor>
void HashMap<Key, Value, Hash_Functor, Equals_Functor>::destroy(HashMapNode* node)
{
     --size_;
     node->key_.~Key();
     node->value_.~Value();
     free(node);
}

