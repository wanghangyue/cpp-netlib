
// Copyright 2010 Dean Michael Berris.
// Copyright 2012 Google, Inc.
// Copyright (c) Glyn Matthews 2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef BUILD_SHARED_LIBS
# define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE thread pool test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <network/concurrency/thread_pool.hpp>
#include <boost/bind.hpp>

using network::concurrency::thread_pool;

// This test specifies the requirements for a thread pool interface. At the
// very least any thread pool implementation should be able to pass the simple
// tests that this unit test requires of thread pools. Ultimately the
// requirements will show up in the Concept documentation, but this test is the
// canonical definition of what a thread pool should look like at least
// syntactically.
//

BOOST_AUTO_TEST_CASE( default_constructor ) {
  thread_pool pool;
  BOOST_CHECK_EQUAL(pool.thread_count(), std::size_t(1));
}

struct foo {
  foo() : val_(0) {}
  void bar(int val) {
    val_ += val;
  }
  int const val() const {
    return val_;
  }
protected:
  int val_;
};

BOOST_AUTO_TEST_CASE( post_work ) {
  foo instance;
  {
    thread_pool pool;
    BOOST_CHECK_NO_THROW(pool.post(boost::bind(&foo::bar, &instance, 1)));
    BOOST_CHECK_NO_THROW(pool.post(boost::bind(&foo::bar, &instance, 2)));
    // require that pool is destroyed here, RAII baby
  }
  BOOST_CHECK_EQUAL(instance.val(), 3);
}
