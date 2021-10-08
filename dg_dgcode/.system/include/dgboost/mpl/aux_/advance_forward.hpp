
#if !defined(BOOST_PP_IS_ITERATING)

///// header body

#ifndef BOOST_MPL_AUX_ADVANCE_FORWARD_HPP_INCLUDED
#define BOOST_MPL_AUX_ADVANCE_FORWARD_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include <dgboost/mpl/next.hpp>
#   include <dgboost/mpl/apply_wrap.hpp>
#endif

#include <dgboost/mpl/aux_/config/use_preprocessed.hpp>

#if    !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER advance_forward.hpp
#   include <dgboost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <dgboost/mpl/limits/unrolling.hpp>
#   include <dgboost/mpl/aux_/nttp_decl.hpp>
#   include <dgboost/mpl/aux_/config/eti.hpp>

#   include <dgboost/preprocessor/iterate.hpp>
#   include <dgboost/preprocessor/cat.hpp>
#   include <dgboost/preprocessor/inc.hpp>

namespace dgboost {} namespace boost = dgboost; namespace dgboost { namespace mpl { namespace aux {

// forward declaration
template< BOOST_MPL_AUX_NTTP_DECL(long, N) > struct advance_forward;

#   define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, BOOST_MPL_LIMIT_UNROLLING, <dgboost/mpl/aux_/advance_forward.hpp>))
#   include BOOST_PP_ITERATE()

// implementation for N that exceeds BOOST_MPL_LIMIT_UNROLLING
template< BOOST_MPL_AUX_NTTP_DECL(long, N) > 
struct advance_forward
{
    template< typename Iterator > struct apply
    {
        typedef typename apply_wrap1<
              advance_forward<BOOST_MPL_LIMIT_UNROLLING>
            , Iterator
            >::type chunk_result_;

        typedef typename apply_wrap1<
              advance_forward<(
                (N - BOOST_MPL_LIMIT_UNROLLING) < 0
                    ? 0
                    : N - BOOST_MPL_LIMIT_UNROLLING
                    )>
            , chunk_result_
            >::type type;
    };
};

}}}

#endif // BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // BOOST_MPL_AUX_ADVANCE_FORWARD_HPP_INCLUDED

///// iteration, depth == 1

// For gcc 4.4 compatability, we must include the
// BOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // BOOST_PP_IS_ITERATING
#if BOOST_PP_ITERATION_DEPTH() == 1
#define i_ BOOST_PP_FRAME_ITERATION(1)

template<>
struct advance_forward< BOOST_PP_FRAME_ITERATION(1) >
{
    template< typename Iterator > struct apply
    {
        typedef Iterator iter0;

#if i_ > 0
#   define BOOST_PP_ITERATION_PARAMS_2 \
    (3,(1, i_, <dgboost/mpl/aux_/advance_forward.hpp>))
#   include BOOST_PP_ITERATE()
#endif
        typedef BOOST_PP_CAT(iter,i_) type;
    };

#if defined(BOOST_MPL_CFG_MSVC_60_ETI_BUG)
    /// ETI workaround
    template<> struct apply<int>
    {
        typedef int type;
    };
#endif
};

#undef i_

///// iteration, depth == 2

#elif BOOST_PP_ITERATION_DEPTH() == 2

#   define AUX778076_ITER_0 BOOST_PP_CAT(iter,BOOST_PP_DEC(BOOST_PP_FRAME_ITERATION(2)))
#   define AUX778076_ITER_1 BOOST_PP_CAT(iter,BOOST_PP_FRAME_ITERATION(2))

        typedef typename next<AUX778076_ITER_0>::type AUX778076_ITER_1;
        
#   undef AUX778076_ITER_1
#   undef AUX778076_ITER_0

#endif // BOOST_PP_ITERATION_DEPTH()
#endif // BOOST_PP_IS_ITERATING
