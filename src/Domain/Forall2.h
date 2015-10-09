//AUTOGENERATED BY genForallN.py
  
#ifndef __DOMAIN_FORALL2_H__
#define __DOMAIN_FORALL2_H__

#include<RAJA/RAJA.hxx>



/******************************************************************
 *  Policy base class, forall2()
 ******************************************************************/

    template<typename LOOP_ORDER, typename POL_I, typename POL_J>
    struct ForallPolicy2 {
      typedef LOOP_ORDER LoopOrder;
      typedef POL_I PolicyI;
      typedef POL_J PolicyJ;
    };


/******************************************************************
 *  Default Executor for forall2()
 ******************************************************************/

    template<typename POLICY_I, typename POLICY_J, typename TI, typename TJ>
    class Forall2Executor {
      public:  
        template<typename BODY>
        inline void operator()(TI const &is_i, TJ const &is_j, BODY const &body) const {
          RAJA::forall<POLICY_I>(is_i, RAJA_LAMBDA(int i){
            RAJA::forall<POLICY_J>(is_j, RAJA_LAMBDA(int j){
              body(i, j);
            });
          });
        }
    };


/******************************************************************
 *  OpenMP Auto-Collapsing Executors for forall2()
 ******************************************************************/

#ifdef _OPENMP

    // OpenMP Executor with collapse(2) for omp_parallel_for_exec, ending with IndexSet
    template<typename SEG_EXEC_POLICY_T>
    class Forall2Executor<RAJA::omp_parallel_for_exec, RAJA::IndexSet::ExecPolicy<RAJA::omp_parallel_for_exec, SEG_EXEC_POLICY_T>, RAJA::RangeSegment, RAJA::IndexSet> {
      public:  
        template<typename BODY>
        inline void operator()(RAJA::RangeSegment const &is_i, RAJA::IndexSet const &is_j, BODY const &body) const {
          using namespace RAJA;
          int const i_start = is_i.getBegin();
          int const i_end   = is_i.getEnd();

          const int num_seg = is_j.getNumSegments();

#pragma omp parallel for schedule(static) collapse(2)
          for(int i = i_start;i < i_end;++ i){
            // collapsed iteration over segments in IndexSet is_j              
             for ( int isi = 0; isi < num_seg; ++isi ) {

                const BaseSegment* iseg = is_j.getSegment(isi);
                SegmentType segtype = iseg->getType();

                switch ( segtype ) {

                   case _RangeSeg_ : {
                      const RangeSegment* tseg =
                         static_cast<const RangeSegment*>(iseg);
                      forall(
                         SEG_EXEC_POLICY_T(),
                         tseg->getBegin(), tseg->getEnd(),
                         [=](int j){body(i,j);}
                      );
                      break;
                   }

          #if 0  // RDH RETHINK
                   case _RangeStrideSeg_ : {
                      const RangeStrideSegment* tseg =
                         static_cast<const RangeStrideSegment*>(iseg);
                      forall(
                         SEG_EXEC_POLICY_T(),
                         tseg->getBegin(), tseg->getEnd(), tseg->getStride(),
                         loop_body
                      );
                      break;
                   }
          #endif

                   case _ListSeg_ : {
                      const ListSegment* tseg =
                         static_cast<const ListSegment*>(iseg);
                      forall(
                         SEG_EXEC_POLICY_T(),
                         tseg->getIndex(), tseg->getLength(),
                         [=](int j){body(i,j);}
                      );
                      break;
                   }

                   default : {
                   }

                }  // switch on segment type

             } // iterate over segments of index set
                            
          }  
        }
    };

    // OpenMP Executor with collapse(2) for omp_parallel_for_exec
    template<>
    class Forall2Executor<RAJA::omp_parallel_for_exec, RAJA::omp_parallel_for_exec, RAJA::RangeSegment, RAJA::RangeSegment> {
      public:  
        template<typename BODY>
        inline void operator()(RAJA::RangeSegment const &is_i, RAJA::RangeSegment const &is_j, BODY const &body) const {
          int const i_start = is_i.getBegin();
          int const i_end   = is_i.getEnd();

          int const j_start = is_j.getBegin();
          int const j_end   = is_j.getEnd();

#pragma omp parallel for schedule(static) collapse(2)
          for(int i = i_start;i < i_end;++ i){
            for(int j = j_start;j < j_end;++ j){
              body(i, j);
          } } 
        }
    };

    // OpenMP Executor with collapse(2) for omp_for_nowait_exec
    template<>
    class Forall2Executor<RAJA::omp_for_nowait_exec, RAJA::omp_for_nowait_exec, RAJA::RangeSegment, RAJA::RangeSegment> {
      public:  
        template<typename BODY>
        inline void operator()(RAJA::RangeSegment const &is_i, RAJA::RangeSegment const &is_j, BODY const &body) const {
          int const i_start = is_i.getBegin();
          int const i_end   = is_i.getEnd();

          int const j_start = is_j.getBegin();
          int const j_end   = is_j.getEnd();

#pragma omp for schedule(static) collapse(2) nowait
          for(int i = i_start;i < i_end;++ i){
            for(int j = j_start;j < j_end;++ j){
              body(i, j);
          } } 
        }
    };


#endif // _OPENMP


/******************************************************************
 *  Permutations layer for forall2()
 ******************************************************************/

      template<typename POLICY, typename TI, typename TJ, typename BODY>
      RAJA_INLINE void forall2_permute(PERM_IJ, TI const &is_i, TJ const &is_j, BODY const &body){
        Forall2Executor<typename POLICY::PolicyI, typename POLICY::PolicyJ, TI, TJ> exec;
        exec(is_i, is_j, RAJA_LAMBDA(int i, int j){
          body(i, j);
        });
      }

      template<typename POLICY, typename TI, typename TJ, typename BODY>
      RAJA_INLINE void forall2_permute(PERM_JI, TI const &is_i, TJ const &is_j, BODY const &body){
        Forall2Executor<typename POLICY::PolicyJ, typename POLICY::PolicyI, TJ, TI> exec;
        exec(is_j, is_i, RAJA_LAMBDA(int j, int i){
          body(i, j);
        });
      }


/******************************************************************
 *  User interface, forall2()
 ******************************************************************/

    template<typename POLICY, typename TI, typename TJ, typename BODY>
    RAJA_INLINE void forall2(TI const &is_i, TJ const &is_j, BODY const &body){
      typedef typename POLICY::LoopOrder L;
      forall2_permute<POLICY, TI, TJ, BODY>(L(), is_i, is_j, body);
    }


  
#endif

