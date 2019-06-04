# ifndef CPPAD_LOCAL_SPARSE_LIST_SETVEC_HPP
# define CPPAD_LOCAL_SPARSE_LIST_SETVEC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/local/define.hpp>
# include <cppad/local/is_pod.hpp>
# include <list>

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace sparse {
/*!
\file list_setvec.hpp
Vector of sets of positive integers stored as singly linked lists
with the element values strictly increasing.
*/
class list_setvec_const_iterator;

/*
Vector of sets of positive integers, each set stored as a singly
linked list.

All the public members for this class are also in the
sparse::pack_setvec and sparse_vecsize classes.
This defines the CppAD vector_of_sets concept.
*/
class list_setvec {
    friend class list_setvec_const_iterator;
/*
-------------------------------------------------------------------------------
$begin list_setvec_member_data$$
$spell
    setvec
    struct
$$

$section class list_setvec: Member Data$$

$head pair_size_t$$
This $code struct$$ is local to the $code list_setvec$$ class.
It is the type used for each entry in a singly linked list
and has the following fields:

$subhead value$$
The is the value of an entry in the list
(for sets, the first entry in the list is a reference count).

$subhead next$$
This is the index in $code data_$$ for the next entry in the list.
If there are no more entries in the list, this value is zero; i.e.,
$code data_[0]$$ is used to represent the end of a list.

$head end_$$
The possible elements in each set are $code 0$$, $code 1$$, ...,
$code end_-1$$

$head number_not_used_$$
Number of elements of the $code data_$$ vector that are not being used.

$head data_not_used_$$
Index in $code data_$$ for the start of the linked list of elements
that are not being used.

$head data_$$
The data for all the singly linked lists.
If $icode%n_set% > 0%$$,
$codei%data[0].value == end_%$$ and
$codei%data[0].next == 0%$$.

$head start_$$
The size of this vector is the number of set; i.e.,
$cref/n_set/SetVector/Vector Operations/n_set/$$.
The starting index for $th i$$ set is $codei%start_[%i%]%$$.
If $codei%start_[%i%] == 0%$$, the i-th set has no elements.
Otherwise it is the index of the reference count for the list.

$subhead Reference Count$$
If $codei%start_[%i%] != 0%$$, $codei%data_[start_[%i%]].value%$$
is the reference count for the $th i$$ list
(not the value of an element in the list).
The reference count must be greater than zero.

$subhead First Element$$
If $codei%start_[%i%] != 0%$$,
$codei%
    %first_index% = data_[start_[%i%]].next
%$$
is the index of the first element in the list.
This must be non-zero because the list is empty.

$subhead Next Element$$
Starting with $icode%index% = %first_index%$$,
while $icode%index% != 0%$$,
The value of the corresponding element in the list is
$codei%data_[%index%].value%$$ (which must be less than $code end_$$).
The $icode index$$ for the next element of the list is
$codei%data_[%index%].next%$$.

$subhead Last Element$$
An index $icode last$$ corresponds to the last element of a list if
$codei%data_[%last%].next == 0%$$.
(Note that $code data_[0].value == end_$$).

$head post_$$
The size of this vector is the number of set; i.e.,
$cref/n_set/SetVector/Vector Operations/n_set/$$.
Starting with $icode%index% = post_[%i%]%$$,
while $icode%index% != 0%$$,
The value of the next element posted to the $th i$$ list is
$codei%data_[%index%].value%$$ (which must be less than $code end_$$).
The $icode index$$ for the next element posted to the $th i$$ list is
$codei%data_[%index%].next%$$.

$head temporary_$$
A temporary vector used by member functions that keeps its capacity
(to re-allocating memory).

$head Source Code$$
$srccode%hpp% */
private:
    struct pair_size_t {size_t value; size_t next; };
    friend bool CppAD::local::is_pod<pair_size_t>(void);
    //
    size_t                   end_;
    size_t                   number_not_used_;
    size_t                   data_not_used_;
    //
    pod_vector<pair_size_t> data_;
    pod_vector<size_t>      start_;
    pod_vector<size_t>      post_;
    pod_vector<size_t>      temporary_;
/* %$$
$end
------------------------------------------------------------------------------
$begin list_setvec_reference_count$$
$spell
    vec
    setvec
    const
$$

$section Return Number of References to a Set$$

$head Syntax$$
$icode%count% = %vec%.reference_count(%i%)%$$

$head vec$$
Is a $code list_setvec$$ object and can be $code const$$.

$head i$$
is the index of the set that we are retrieving the references for.

$head count$$
is the reference count.

$head Prototype$$
$srccode%hpp% */
private:
    size_t reference_count(size_t i) const
/* %$$
$end
*/
    {   // start data index
        size_t start = start_[i];
        if( start == 0 )
            return 0;
        //
        // reference count
        return data_[start].value;
    }
/*
------------------------------------------------------------------------------
$begin list_setvec_drop$$
$spell
    vec
    decremented
$$
$section Drop a Set That is No Longer Being Used$$

$head Syntax$$
$icode%not_used% = %vec%.drop(%i%)%$$

$head i$$
is the index of the set that is dropped.

$head reference_count$$
if the set is non-empty, the
$cref/reference count/list_setvec_member_data/start_/Reference Count/$$
is decremented.

$head start_$$
The value $codei%start_[%i%]%$$ is set to zero; i.e.,
the $th i$$ set is empty after the call.

$head post_$$
The value $codei%post_[%i%]%$$ is set to zero; i.e.,
any postings to the list are also dropped.

$head data_not_used_$$
The elements of $code data_$$ were information for the $th i$$ set,
and are no longer being used, are added to the linked list starting at
$code data_not_used$$.
This includes both set elements and postings.

$head not_used$$
is the number of elements of $code data_$$ that were begin used for the
$th i$$ set and are no longer being used; i.e., the number of elements
moved to $code data_not_used$$.

$head Prototype$$
$srccode%hpp% */
private:
    size_t drop(size_t i)
/* %$$
$end
*/
    {   // inialize count of addition elements not being used.
        size_t number_drop = 0;

        // the elements in the post list will no longer be used
        size_t post = post_[i];
        if( post != 0 )
        {   // drop this posting
            post_[i]    = 0;
            //
            // count elements in this posting
            ++number_drop;
            size_t previous = post;
            size_t next     = data_[previous].next;
            while( next != 0 )
            {   previous = next;
                next     = data_[previous].next;
                ++number_drop;
            }
            //
            // add the posting elements to data_not_used_
            data_[previous].next = data_not_used_;
            data_not_used_       = post;
        }

        // check for empty set
        size_t start = start_[i];
        if( start == 0 )
            return number_drop;

        // decrement reference counter
        CPPAD_ASSERT_UNKNOWN( data_[start].value > 0 );
        data_[start].value--;

        // set this set to empty
        start_[i] = 0;

        // If new reference count is positive, the list corresponding to
        // start is still being used.
        if( data_[start].value > 0 )
            return number_drop;

        //
        // count elements representing this set
        ++number_drop;
        size_t previous = start;
        size_t next     = data_[previous].next;
        while( next != 0 )
        {   previous = next;
            next     = data_[previous].next;
            ++number_drop;
        }
        //
        // add representing this set to data_not_used_
        data_[previous].next = data_not_used_;
        data_not_used_       = start;
        //
        return number_drop;
    }
/*
------------------------------------------------------------------------------
$begin list_setvec_get_data_index$$
$spell
    decremented
    setvec
    vec
$$

$section class list_setvec: Get a New List Pair$$

$head Syntax$$
$icode%index% = %vec%.get_data_index()%$$

$head vec$$
Is a $code list_setvec$$ object.

$head data_not_used_$$
If the input value of $code data_not_used_$$ is zero,
it is not changed.
Otherwise, the index for the element at the front of that list is returned.
In this case,
$code data_not_used$$ is advanced to the next element in that list.

$head number_not_used_$$
If the input value of $code data_not_used_$$ is zero,
$code number_not_used_$$ is not changed.
Otherwise it is decremented by one.

$head index$$
If the input value of $code data_not_used_$$ is zero,
the size of $code data_$$ is increased by one and index corresponding
to the end of $code data_$$ is returned.
Otherwise, the input value for $code data_not_used_$$ is returned.

$head Prototype$$
$srccode%hpp% */
private:
    size_t get_data_index(void)
/* %$$
$end
*/
    {   size_t index;
        if( data_not_used_ > 0 )
        {   CPPAD_ASSERT_UNKNOWN( number_not_used_ > 0 );
            --number_not_used_;
            index          = data_not_used_;
            data_not_used_ = data_[index].next;
        }
        else
        {   index = data_.extend(1);
        }
        return index;
    }
/*
-------------------------------------------------------------------------------
$begin list_setvec_check_data_structure$$
$spell
    setvec
    vec
    const
$$

$section class list_setvec: Check Data Structure$$

$head Syntax$$
$icode%vec%.check_data_structure()%$$

$head vec$$
Is a $code list_setvec$$ object that is effectively const.
It is not declared const because the data structure is modified and
then restored.

$head NDEBUG$$
If $code NDEBUG$$ is defined, the routine does nothing.
Otherwise, if an error is found in the data structure,
a $code CPPAD_ASSERT_UNKNOWN$$ is generated.

$head Prototype$$
$srccode%hpp% */
private:
    void check_data_structure(void)
/* %$$
$end
*/
# ifdef NDEBUG
    {   return; }
# else
    {   // number of sets
        CPPAD_ASSERT_UNKNOWN( post_.size() == start_.size() );
        size_t n_set = start_.size();
        if( n_set == 0 )
        {   CPPAD_ASSERT_UNKNOWN( end_ == 0 );
            CPPAD_ASSERT_UNKNOWN( number_not_used_ == 0 );
            CPPAD_ASSERT_UNKNOWN( data_not_used_ == 0 );
            CPPAD_ASSERT_UNKNOWN( data_.size() == 0 );
            CPPAD_ASSERT_UNKNOWN( start_.size() == 0 );
            return;
        }
        // check data index zero
        CPPAD_ASSERT_UNKNOWN( data_[0].value == end_ );
        CPPAD_ASSERT_UNKNOWN( data_[0].next  == 0  );
        // -----------------------------------------------------------
        // save a copy of the reference counters in temporary_
        temporary_.resize(n_set);
        for(size_t i = 0; i < n_set; i++)
            temporary_[i] = reference_count(i);
        // -----------------------------------------------------------
        // Initialize number of entries in data used by sets and posts.
        // Start with 1 for data_[0].
        size_t number_used_by_sets = 1;
        // -----------------------------------------------------------
        // count the number of entries in data_ that are used by sets
        for(size_t i = 0; i < n_set; i++)
        {   size_t start = start_[i];
            if( start > 0 )
            {   // check structure for this non-empty set
                size_t reference_count = data_[start].value;
                size_t next            = data_[start].next;
                CPPAD_ASSERT_UNKNOWN( reference_count > 0 );
                CPPAD_ASSERT_UNKNOWN( next != 0 );
                CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
                //
                // decrement the reference counter
                data_[start].value--;
                //
                // count the entries when find last reference
                if( data_[start].value == 0 )
                {
                    // restore reference count
                    data_[start].value = temporary_[i];

                    // number of data entries used for this set
                    number_used_by_sets += number_elements(i) + 1;
                    /*
                    number of elements checks that value < end_
                    .resizeeach pair in the list except for the start pair
                    and the pair with index zero.
                    */
                }
            }
        }
        // ------------------------------------------------------------------
        // count the number of entries in data_ that are used by posts
        size_t number_used_by_posts = 0;
        for(size_t i = 0; i < n_set; i++)
        {   size_t post = post_[i];
            if( post > 0 )
            {   size_t value = data_[post].value;
                size_t next  = data_[post].next;
                CPPAD_ASSERT_UNKNOWN( value < end_ );
                //
                while( value < end_ )
                {   ++number_used_by_posts;
                    value = data_[next].value;
                    next  = data_[next].next;
                }
            }
        }
        // ------------------------------------------------------------------
        // count number of entries in data_not_used_
        size_t count = 0;
        size_t next = data_not_used_;
        while( next != 0 )
        {   ++count;
            next = data_[next].next;
        }
        CPPAD_ASSERT_UNKNOWN( number_not_used_ == count );
        // ------------------------------------------------------------------
        size_t number_used = number_used_by_sets + number_used_by_posts;
        CPPAD_ASSERT_UNKNOWN(
            number_used + number_not_used_ == data_.size()
        );
        return;
    }
# endif
// ===========================================================================
public:
    /// declare a const iterator
    typedef list_setvec_const_iterator const_iterator;
    // -----------------------------------------------------------------
    /*!
    Default constructor (no sets)
    */
    list_setvec(void) :
    end_(0)              ,
    number_not_used_(0)  ,
    data_not_used_(0)    ,
    data_(0)             ,
    start_(0)            ,
    post_(0)
    { }
    // -----------------------------------------------------------------
    /// Destructor
    ~list_setvec(void)
    {   check_data_structure();
    }
    // -----------------------------------------------------------------
    /*!
    Using copy constructor is a programing (not user) error

    \param v
    vector of sets that we are attempting to make a copy of.
    */
    list_setvec(const list_setvec& v)
    {   // Error: Probably a list_setvec argument has been passed by value
        CPPAD_ASSERT_UNKNOWN(false);
    }
    // -----------------------------------------------------------------
    /*!
    Assignement operator.

    \param other
    this list_setvec with be set to a deep copy of other.

    \par vector_of_sets
    This public member function is not yet part of
    the vector_of_sets concept.
    */
    void operator=(const list_setvec& other)
    {   end_             = other.end_;
        number_not_used_ = other.number_not_used_;
        data_not_used_   = other.data_not_used_;
        data_            = other.data_;
        start_           = other.start_;
        post_            = other.post_;
    }
    // -----------------------------------------------------------------
    /*!
    swap (used by move semantics version of ADFun assignment operator)

    \param other
    this list_setvec with be swapped with other.

    \par vector_of_sets
    This public member function is not yet part of
    the vector_of_sets concept.
    */
    void swap(list_setvec& other)
    {   // size_t objects
        std::swap(end_             , other.end_);
        std::swap(number_not_used_ , other.number_not_used_);
        std::swap(data_not_used_   , other.data_not_used_);

        // pod_vectors
        data_.swap(       other.data_);
        start_.swap(      other.start_);
        post_.swap(       other.post_);
        temporary_.swap(  other.temporary_);
    }
    // -----------------------------------------------------------------
    /*!
    Start a new vector of sets.

    \param n_set
    is the number of sets in this vector of sets.
    \li
    If n_set is zero, any memory currently allocated for this object
    is freed.
    \li
    If n_set is non-zero, a vector of n_set sets is created and all
    the sets are initilaized as empty.

    \param end
    is the maximum element plus one (the minimum element is 0).
    If n_set is zero, end must also be zero.
    */
    void resize(size_t n_set, size_t end)
    {   check_data_structure();

        if( n_set == 0 )
        {   CPPAD_ASSERT_UNKNOWN( end == 0 );
            //
            // restore object to start after constructor
            // (no memory allocated for this object)
            data_.clear();
            start_.clear();
            post_.clear();
            number_not_used_  = 0;
            data_not_used_    = 0;
            end_              = 0;
            //
            return;
        }
        end_                   = end;
        //
        start_.resize(n_set);
        post_.resize(n_set);
        //
        for(size_t i = 0; i < n_set; i++)
        {   start_[i] = 0;
            post_[i]  = 0;
        }
        //
        // last element, marks the end for all lists
        data_.resize(1);
        data_[0].value    = end_;
        data_[0].next     = 0;
        //
        number_not_used_  = 0;
        data_not_used_    = 0;
    }
    // -----------------------------------------------------------------
    /*!
    Count number of elements in a set.

    \param i
    is the index of the set we are counting the elements of.

    \par
    number of elements checks that value < end_ for each element of the set.
    */
    size_t number_elements(size_t i) const
    {   CPPAD_ASSERT_UNKNOWN( post_[i] == 0 );

        // check if the set is empty
        size_t start   = start_[i];
        if( start == 0 )
            return 0;

        // initialize counter
        size_t count   = 0;

        // advance to the first element in the set
        size_t next    = data_[start].next;
        while( next != 0 )
        {   CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
            count++;
            next  = data_[next].next;
        }
        CPPAD_ASSERT_UNKNOWN( count > 0 );
        return count;
    }
    /*!
    Post an element for delayed addition to a set.

    \param i
    is the index for this set in the vector of sets.

    \param element
    is the value of the element that we are posting.
    The same element may be posted multiple times.

    \par
    It is faster to post multiple elements to set i and then call
    process_post(i) then to add each element individually.
    It is an error to call any member function,
    that depends on the value of set i,
    before processing the posts to set i.
    */
    void post_element(size_t i, size_t element)
    {   CPPAD_ASSERT_UNKNOWN( i < start_.size() );
        CPPAD_ASSERT_UNKNOWN( element < end_ );

        // put element at the front of this list
        size_t next         = post_[i];
        size_t post         = get_data_index();
        post_[i]            = post;
        data_[post].value   = element;
        data_[post].next    = next;

        return;
    }
    // -----------------------------------------------------------------
    /*!
    process post entries for a specific set.

    \param i
    index of the set for which we are processing the post entries.

    \par post_
    Upon call, post_[i] is location in data_ of the elements that get
    added to the i-th set.  Upon return, post_[i] is zero.
    */
    void process_post(size_t i)
    {   // post
        size_t post = post_[i];
        //
        // check if there are no elements to process
        if( post == 0 )
            return;
        //
        // check if there is only one element to process
        size_t next  = data_[post].next;
        if( next == 0 )
        {   // done with this posting
            size_t value     = data_[post].value;
            post_[i]         = 0;
            data_[post].next = data_not_used_;
            data_not_used_   = post;
            ++number_not_used_;
            //
            add_element(i, value);
            //
            return;
        }
        //
        // copy posting to temporary_
        temporary_.resize(0);
        size_t previous  = post;
        size_t value     = data_[previous].value;
        CPPAD_ASSERT_UNKNOWN( value < end_ );
        temporary_.push_back(value);
        while( next != 0 )
        {   previous = next;
            value    = data_[previous].value;
            CPPAD_ASSERT_UNKNOWN( value < end_ );
            temporary_.push_back(value);
            next     = data_[previous].next;
        }
        size_t number_post = temporary_.size();
        //
        // done with this posting
        post_[i]              = 0;
        data_[previous].next  = data_not_used_;
        data_not_used_        = post;
        number_not_used_     += number_post;;
        //
        // sort temporary_
        CPPAD_ASSERT_UNKNOWN( number_post > 1 );
        std::sort( temporary_.data(), temporary_.data() + number_post);
        // posting is the set { temporary_[0], ... , [number_post-1] }
        // -------------------------------------------------------------------
        // put union of posting and set i in
        // temporary_[number_post], ... , temporary_[ temporary_.size()-1 ]
        //
        size_t i_next  = start_[i];
        size_t i_value = end_;
        if( i_next > 0 )
        {   // skip reference count
            i_next  = data_[i_next].next;
            i_value = data_[i_next].value;
        }
        bool   post_is_subset = true;
        size_t previous_post = end_;
        for(size_t j =0; j < number_post; ++j)
        {   size_t post_value = temporary_[j];
            CPPAD_ASSERT_UNKNOWN( post_value < end_ );
            while( i_value < post_value )
            {   // i_value is in union
                temporary_.push_back(i_value);
                i_next  = data_[i_next].next;
                i_value = data_[i_next].value;
            }
            if( i_value == post_value )
            {   i_next  = data_[i_next].next;
                i_value = data_[i_next].value;
            }
            else
                post_is_subset = false;
            //
            if( previous_post != post_value )
            {   // post_value is in union
                temporary_.push_back(post_value);
            }
            previous_post = post_value;
        }
        // check if posting is a subset of set i
        if( post_is_subset )
            return;
        //
        // rest of elements in set i
        while( i_value < end_ )
        {   temporary_.push_back(i_value);
            i_next  = data_[i_next].next;
            i_value = data_[i_next].value;
        }

        // adjust number_not_used_
        size_t number_drop = drop(i);
        number_not_used_  += number_drop;

        // put new set in linked list for set i
        CPPAD_ASSERT_UNKNOWN( temporary_.size() >= number_post + 1 );
        size_t index        = get_data_index();
        start_[i]           = index; // start for the union
        data_[index].value  = 1;    // reference count for the union
        for(size_t j = number_post; j < temporary_.size(); ++j)
        {   next              = get_data_index();
            data_[index].next = next;
            data_[next].value = temporary_[j]; // next element in union
            index             = next;
        }
        data_[index].next = 0; // end of union
        //
        return;
    }
    // -----------------------------------------------------------------
    /*!
    Add one element to a set.

    \param i
    is the index for this set in the vector of sets.

    \param element
    is the element we are adding to the set.
    */
    void add_element(size_t i, size_t element)
    {   CPPAD_ASSERT_UNKNOWN( i   < start_.size() );
        CPPAD_ASSERT_UNKNOWN( element < end_ );

        // check for case where starting set is empty
        size_t start = start_[i];
        if( start == 0 )
        {   start              = get_data_index();
            start_[i]          = start;
            data_[start].value = 1; // reference count
            //
            size_t next        = get_data_index();
            data_[start].next  = next;
            //
            data_[next].value  = element;
            data_[next].next   = 0;
            return;
        }
        //
        // start of set with this index
        size_t previous = start_[i];
        //
        // first entry in this set
        size_t next     = data_[previous].next;
        size_t value    = data_[next].value;
        //
        // locate place to insert this element
        while( value < element )
        {   previous = next;
            next     = data_[next].next;
            value = data_[next].value;
        }
        //
        // check for case where element is in the set
        if( value == element )
            return;
        //
        //
        // check for case where this is the only reference to this set
        CPPAD_ASSERT_UNKNOWN( element < value );
        if( data_[start].value == 1 )
        {   size_t insert         = get_data_index();
            data_[insert].next    = next;
            data_[insert].value   = element;
            data_[previous].next  = insert;
            //
            return;
        }
        //
        // must make a separate copy with new element inserted
        CPPAD_ASSERT_UNKNOWN( data_[start].value > 1 );
        data_[start].value--;   // reverence counter for old list
        //
        size_t start_new       = get_data_index();
        data_[start_new].value = 1;         // reference counter for new list
        size_t previous_new    = start_new;
        //
        // start of old set with this index
        previous  = start_[i];
        //
        // first entry in old set
        next    = data_[previous].next;
        value   = data_[next].value;
        //
        // locate place to insert this element
        while( value < element )
        {   // copy to new list
            size_t next_new          = get_data_index();
            data_[previous_new].next = next_new;
            data_[next_new].value    = value;
            previous_new             = next_new;
            //
            // get next value
            previous = next;
            next     = data_[next].next;
            value = data_[next].value;
        }
        CPPAD_ASSERT_UNKNOWN( element < value );
        //
        // insert the element
        size_t next_new          = get_data_index();
        data_[previous_new].next = next_new;
        data_[next_new].value    = element;
        previous_new             = next_new;
        //
        // copy rest of the old set
        while( value < end_ )
        {   // copy to new list
            next_new                 = get_data_index();
            data_[previous_new].next = next_new;
            data_[next_new].value    = value;
            previous_new             = next_new;
            //
            // get next value
            previous = next;
            next     = data_[next].next;
            value = data_[next].value;
        }
        CPPAD_ASSERT_UNKNOWN( next == 0 );
        data_[previous_new].next = 0;
        //
        // hook up new list
        start_[i] = start_new;
        return;
    }
    // -----------------------------------------------------------------
    /*!
    check an element is in a set.

    \param i
    is the index for this set in the vector of sets.

    \param element
    is the element we are checking to see if it is in the set.
    */
    bool is_element(size_t i, size_t element) const
    {   CPPAD_ASSERT_UNKNOWN( post_[i] == 0 );
        CPPAD_ASSERT_UNKNOWN( element < end_ );
        //
        size_t start = start_[i];
        if( start == 0 )
            return false;
        //
        size_t next  = data_[start].next;
        size_t value = data_[next].value;
        while( value < element )
        {   next  = data_[next].next;
            value = data_[next].value;
        }
        return element == value;
    }
    // -----------------------------------------------------------------
    /*!
    Assign the empty set to one of the sets.

    \param target
    is the index of the set we are setting to the empty set.

    \par number_not_used_
    increments this value by additional number of data_ elements that are
    no longer being used.
    */
    void clear(size_t target)
    {   CPPAD_ASSERT_UNKNOWN( target < start_.size() );

        // adjust number_not_used_
        size_t number_drop = drop(target);
        number_not_used_  += number_drop;

        return;
    }
    // -----------------------------------------------------------------
    /*!
    Assign one set equal to another set.

    \param this_target
    is the index in this list_setvec object of the set being assinged.

    \param other_source
    is the index in the other list_setvec object of the
    set that we are using as the value to assign to the target set.

    \param other
    is the other list_setvec object (which may be the same as this
    list_setvec object). This must have the same value for end_.

    \par number_not_used_
    increments this value by additional number of elements not being used.
    */
    void assignment(
        size_t               this_target  ,
        size_t               other_source ,
        const list_setvec&   other        )
    {   CPPAD_ASSERT_UNKNOWN( other.post_[ other_source ] == 0 );
        //
        CPPAD_ASSERT_UNKNOWN( this_target  <   start_.size()        );
        CPPAD_ASSERT_UNKNOWN( other_source <   other.start_.size()  );
        CPPAD_ASSERT_UNKNOWN( end_        == other.end_   );

        // check if we are assigning a set to itself
        if( (this == &other) & (this_target == other_source) )
            return;

        // set depending on cases below
        size_t this_start;

        // If this and other are the same, use another reference to same list
        size_t other_start = other.start_[other_source];
        if( this == &other )
        {   this_start = other_start;
            if( other_start != 0 )
            {   data_[other_start].value++; // increment reference count
                CPPAD_ASSERT_UNKNOWN( data_[other_start].value > 1 );
            }
        }
        else if( other_start  == 0 )
        {   this_start = 0;
        }
        else
        {   // make a copy of the other list in this list_setvec
            this_start        = get_data_index();
            size_t this_next  = get_data_index();
            data_[this_start].value = 1; // reference count
            data_[this_start].next  = this_next;
            //
            size_t next  = other.data_[other_start].next;
            CPPAD_ASSERT_UNKNOWN( next != 0 );
            while( next != 0 )
            {   data_[this_next].value = other.data_[next].value;
                next                   = other.data_[next].next;
                if( next == 0 )
                    data_[this_next].next = 0;
                else
                {   size_t tmp = get_data_index();
                    data_[this_next].next = tmp;
                    this_next             = tmp;
                }
            }
        }

        // adjust number_not_used_
        size_t number_drop = drop(this_target);
        number_not_used_  += number_drop;

        // set the new start value for this_target
        start_[this_target] = this_start;

        return;
    }
    // -----------------------------------------------------------------
    /*!
    Assign a set equal to the union of two other sets.

    \param this_target
    is the index in this list_setvec object of the set being assinged.

    \param this_left
    is the index in this list_setvec object of the
    left operand for the union operation.
    It is OK for this_target and this_left to be the same value.

    \param other_right
    is the index in the other list_setvec object of the
    right operand for the union operation.
    It is OK for this_target and other_right to be the same value.

    \param other
    is the other list_setvec object (which may be the same as this
    list_setvec object).
    */
    void binary_union(
        size_t                  this_target  ,
        size_t                  this_left    ,
        size_t                  other_right  ,
        const list_setvec&      other        )
    {   CPPAD_ASSERT_UNKNOWN( post_[this_left] == 0 );
        CPPAD_ASSERT_UNKNOWN( other.post_[ other_right ] == 0 );
        //
        CPPAD_ASSERT_UNKNOWN( this_target < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( this_left   < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( other_right < other.start_.size()   );
        CPPAD_ASSERT_UNKNOWN( end_        == other.end_           );

        // start indices for left and right sets
        size_t start_left    = start_[this_left];
        size_t start_right   = other.start_[other_right];

        // if right is empty, the result is the left set
        if( start_right == 0 )
        {   assignment(this_target, this_left, *this);
            return;
        }
        // if left is empty, the result is the right set
        if( start_left == 0 )
        {   assignment(this_target, other_right, other);
            return;
        }
        // if niether case holds, then both left and right are non-empty
        CPPAD_ASSERT_UNKNOWN( reference_count(this_left) > 0 );
        CPPAD_ASSERT_UNKNOWN( other.reference_count(other_right) > 0 );

        // we will use temparary_ for temporary storage of the union
        temporary_.resize(0);

        // for left next and value
        size_t next_left   = data_[start_left].next;
        size_t value_left  = data_[next_left].value;

        // right next and value
        size_t next_right  = other.data_[start_right].next;
        size_t value_right = other.data_[next_right].value;

        // both left and right set are non-empty
        CPPAD_ASSERT_UNKNOWN( value_left < end_ && value_right < end_ );

        // flag that detects if left is or right is a subset of the other
        bool left_is_subset  = true;
        bool right_is_subset = true;

        while( (value_left < end_) & (value_right < end_) )
        {   if( value_left == value_right )
            {   // value is in both sets
                temporary_.push_back(value_left);
                //
                // advance left
                next_left  = data_[next_left].next;
                value_left = data_[next_left].value;
                //
                // advance right
                next_right  = other.data_[next_right].next;
                value_right = other.data_[next_right].value;
            }
            else if( value_left < value_right )
            {   // need a value from left that is not in right
                left_is_subset = false;
                temporary_.push_back(value_left);
                //
                // advance left to its next element
                next_left  = data_[next_left].next;
                value_left = data_[next_left].value;
            }
            else
            {   CPPAD_ASSERT_UNKNOWN( value_right < value_left )
                // need a value from right that is not in left
                right_is_subset = false;
                temporary_.push_back(value_right);
                //
                // advance right to its next element
                next_right  = other.data_[next_right].next;
                value_right = other.data_[next_right].value;
            }
        }
        right_is_subset &= value_right == end_;
        left_is_subset  &= value_left  == end_;
        //
        // check right first in case they are equal will do this assignment
        if( right_is_subset )
        {   assignment(this_target, this_left, *this);
            return;
        }
        if( left_is_subset )
        {   assignment(this_target, other_right, other);
            return;
        }
        while( value_left < end_ )
        {   CPPAD_ASSERT_UNKNOWN( value_right == end_);
            temporary_.push_back(value_left);
            next_left  = data_[next_left].next;
            value_left = data_[next_left].value;
        }
        while( value_right < end_ )
        {   CPPAD_ASSERT_UNKNOWN( value_left == end_);
            temporary_.push_back(value_right);
            next_right  = other.data_[next_right].next;
            value_right = other.data_[next_right].value;
        }

        // adjust number_not_used_
        size_t number_drop = drop(this_target);
        number_not_used_  += number_drop;

        // put new set in linked for this_target
        CPPAD_ASSERT_UNKNOWN( temporary_.size() >= 2 );
        size_t index        = get_data_index();
        start_[this_target] = index; // start for the union
        data_[index].value  = 1;    // reference count for the union
        for(size_t i = 0; i < temporary_.size(); ++i)
        {   size_t next       = get_data_index();
            data_[index].next = next;
            data_[next].value = temporary_[i]; // next element in union
            index             = next;
        }
        data_[index].next = 0; // end of union

        return;
    }
    // -----------------------------------------------------------------
    /*!
    Assign a set equal to the intersection of two other sets.

    \param this_target
    is the index in this list_setvec object of the set being assinged.

    \param this_left
    is the index in this list_setvec object of the
    left operand for the intersection operation.
    It is OK for this_target and this_left to be the same value.

    \param other_right
    is the index in the other list_setvec object of the
    right operand for the intersection operation.
    It is OK for this_target and other_right to be the same value.

    \param other
    is the other list_setvec object (which may be the same as this
    list_setvec object).
    */
    void binary_intersection(
        size_t                  this_target  ,
        size_t                  this_left    ,
        size_t                  other_right  ,
        const list_setvec&      other        )
    {   CPPAD_ASSERT_UNKNOWN( post_[this_left] == 0 );
        CPPAD_ASSERT_UNKNOWN( other.post_[ other_right ] == 0 );
        //
        CPPAD_ASSERT_UNKNOWN( this_target < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( this_left   < start_.size()         );
        CPPAD_ASSERT_UNKNOWN( other_right < other.start_.size()   );
        CPPAD_ASSERT_UNKNOWN( end_        == other.end_           );

        // start indices for left and right sets
        size_t start_left    = start_[this_left];
        size_t start_right   = other.start_[other_right];

        // if left or right is empty, the result is empty
        if( (start_left == 0) | (start_right == 0) )
        {   clear(this_target);
            return;
        }
        // if niether case holds, then both left and right are non-empty
        CPPAD_ASSERT_UNKNOWN( reference_count(this_left) > 0 );
        CPPAD_ASSERT_UNKNOWN( other.reference_count(other_right) > 0 );

        // we will use temparary_ for temporary storage of the intersection
        temporary_.resize(0);

        // left next and value
        size_t next_left   = data_[start_left].next;
        size_t value_left  = data_[next_left].value;

        // right next and value
        size_t next_right  = other.data_[start_right].next;
        size_t value_right = other.data_[next_right].value;

        // both left and right set are non-empty
        CPPAD_ASSERT_UNKNOWN( value_left < end_ && value_right < end_ );

        // flag that detects if left is or right is a subset of the other
        bool left_is_subset  = true;
        bool right_is_subset = true;

        while( (value_left < end_) & (value_right < end_) )
        {   if( value_left == value_right )
            {   // value is in both sets
                temporary_.push_back(value_left);
                //
                // advance left
                next_left  = data_[next_left].next;
                value_left = data_[next_left].value;
                //
                // advance right
                next_right  = other.data_[next_right].next;
                value_right = other.data_[next_right].value;
            }
            else if( value_left < value_right )
            {   // there is a value in left that is not in right
                left_is_subset = false;
                //
                // advance left to its next element
                next_left  = data_[next_left].next;
                value_left = data_[next_left].value;
            }
            else
            {   CPPAD_ASSERT_UNKNOWN( value_right < value_left )
                // there is a value in right that is not in left
                right_is_subset = false;
                //
                // advance right to its next element
                next_right  = other.data_[next_right].next;
                value_right = other.data_[next_right].value;
            }
        }
        right_is_subset &= value_right == end_;
        left_is_subset  &= value_left  == end_;
        //
        // check left first in case they are equal will do this assignment
        if( left_is_subset )
        {   assignment(this_target, this_left, *this);
            return;
        }
        if( right_is_subset )
        {   assignment(this_target, other_right, other);
            return;
        }

        // adjust number_not_used_
        size_t number_drop = drop(this_target);
        number_not_used_  += number_drop;

        // check for empty result
        if( temporary_.size() == 0 )
            return;

        // put new set in linked for this_target
        size_t index        = get_data_index();
        start_[this_target] = index; // start for the union
        data_[index].value  = 1;    // reference count for the union
        for(size_t i = 0; i < temporary_.size(); ++i)
        {   size_t next       = get_data_index();
            data_[index].next = next;
            data_[next].value = temporary_[i]; // next element in union
            index             = next;
        }
        data_[index].next = 0; // end of union

        return;
    }
    // -----------------------------------------------------------------
    /*! Fetch n_set for vector of sets object.

    \return
    Number of from sets for this vector of sets object
    */
    size_t n_set(void) const
    {   return start_.size(); }
    // -----------------------------------------------------------------
    /*! Fetch end for this vector of sets object.

    \return
    is the maximum element value plus one (the minimum element value is 0).
    */
    size_t end(void) const
    {   return end_; }
    // -----------------------------------------------------------------
    /*! Amount of memory used by this vector of sets

    \return
    The amount of memory in units of type unsigned char memory.
    */
    size_t memory(void) const
    {   return data_.capacity() * sizeof(pair_size_t);
    }
    /*!
    Print the vector of sets (used for debugging)
    */
    void print(void) const;
};
// =========================================================================
/*!
cons_iterator for one set of positive integers in a list_setvec object.

All the public members for this class are also in the
sparse::pack_setvec_const_iterator and sparse::svec_setvec_const_iterator classes.
This defines the CppAD vector_of_sets iterator concept.
*/
class list_setvec_const_iterator {
private:
    /// type used by list_setvec to represent one element of the list
    typedef list_setvec::pair_size_t pair_size_t;

    /// data for the entire vector of sets
    const pod_vector<pair_size_t>& data_;

    /// Possible elements in a list are 0, 1, ..., end_ - 1;
    const size_t                   end_;

    /// next element in the singly linked list
    /// (next_pair_.value == end_ for past end of list)
    pair_size_t                    next_pair_;
public:
    /// construct a const_iterator for a list in a list_setvec object
    list_setvec_const_iterator (const list_setvec& list, size_t i)
    :
    data_( list.data_ )    ,
    end_ ( list.end_ )
    {   CPPAD_ASSERT_UNKNOWN( list.post_[i] == 0 );
        //
        size_t start = list.start_[i];
        if( start == 0 )
        {   next_pair_.next  = 0;
            next_pair_.value = end_;
        }
        else
        {   // value for this entry is reference count for list
            CPPAD_ASSERT_UNKNOWN( data_[start].value > 0 );

            // data index where list truely starts
            size_t next = data_[start].next;
            CPPAD_ASSERT_UNKNOWN( next != 0 );

            // true first entry in the list
            next_pair_ = data_[next];
            CPPAD_ASSERT_UNKNOWN( next_pair_.value < end_ );
        }
    }

    /// advance to next element in this list
    list_setvec_const_iterator& operator++(void)
    {   next_pair_  = data_[next_pair_.next];
        return *this;
    }

    /// obtain value of this element of the set of positive integers
    /// (end_ for no such element)
    size_t operator*(void)
    {   return next_pair_.value; }
};
// =========================================================================
/*!
Print the vector of sets (used for debugging)
*/
inline void list_setvec::print(void) const
{   std::cout << "list_setvec:\n";
    for(size_t i = 0; i < n_set(); i++)
    {   std::cout << "set[" << i << "] = {";
        const_iterator itr(*this, i);
        while( *itr != end() )
        {   std::cout << *itr;
            if( *(++itr) != end() )
                std::cout << ",";
        }
        std::cout << "}\n";
    }
    return;
}

/*!
Copy a user vector of sets sparsity pattern to an internal list_setvec object.

\tparam SetVector
is a simple vector with elements of type std::set<size_t>.

\param internal
The input value of sparisty does not matter.
Upon return it contains the same sparsity pattern as user
(or the transposed sparsity pattern).

\param user
sparsity pattern that we are placing internal.

\param n_set
number of sets (rows) in the internal sparsity pattern.

\param end
end of set value (number of columns) in the interanl sparsity pattern.

\param transpose
if true, the user sparsity patter is the transposed.

\param error_msg
is the error message to display if some values in the user sparstiy
pattern are not valid.
*/
template<class SetVector>
void sparsity_user2internal(
    list_setvec&            internal  ,
    const SetVector&        user      ,
    size_t                  n_set     ,
    size_t                  end       ,
    bool                    transpose ,
    const char*             error_msg )
{
# ifndef NDEBUG
    if( transpose )
        CPPAD_ASSERT_KNOWN( end == size_t( user.size() ), error_msg);
    if( ! transpose )
        CPPAD_ASSERT_KNOWN( n_set == size_t( user.size() ), error_msg);
# endif

    // iterator for user set
    std::set<size_t>::const_iterator itr;

    // size of internal sparsity pattern
    internal.resize(n_set, end);

    if( transpose )
    {   // transposed pattern case
        for(size_t j = 0; j < end; j++)
        {   itr = user[j].begin();
            while(itr != user[j].end())
            {   size_t i = *itr++;
                CPPAD_ASSERT_KNOWN(i < n_set, error_msg);
                internal.add_element(i, j);
            }
        }
    }
    else
    {   for(size_t i = 0; i < n_set; i++)
        {   itr = user[i].begin();
            while(itr != user[i].end())
            {   size_t j = *itr++;
                CPPAD_ASSERT_KNOWN( j < end, error_msg);
                internal.add_element(i, j);
            }
        }
    }
    return;
}

} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE

// =========================================================================
// Tell pod_vector class that each pair_size_t is plain old data and hence
// the corresponding constructor need not be called.
namespace CppAD { namespace local {
    template <> inline bool
    is_pod<sparse::list_setvec::pair_size_t>(void)
    {   return true; }
} }

# endif
