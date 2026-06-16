! RUN: bbc -emit-hlfir --wrap-unstructured-constructs-in-execute-region %s -o - | FileCheck %s

! An unstructured IF inside a DO is self-contained, so isUnstructured does
! not propagate to the DO. The DO lowers as fir.do_loop and the IF's blocks
! are wrapped in an scf.execute_region inside its body.
subroutine wrapped_unstructured(n, a)
  integer :: n, i
  real :: a(n)
  do i = 1, n
    if (a(i) > 0.0) stop
    a(i) = real(i)
  end do
end subroutine

! CHECK-LABEL: func.func @_QPwrapped_unstructured
! CHECK:         fir.do_loop
! CHECK:           scf.execute_region no_inline {
! CHECK:             cf.cond_br
! CHECK:             fir.call @_FortranAStopStatement
! CHECK:             fir.unreachable
! CHECK:             scf.yield
! CHECK:           }
! CHECK:           fir.result

! Unstructured DO with a GOTO targeting a label outside the construct that
! is reachable on a path distinct from the loop's natural exit: not wrapped.
! The GOTO jumps past the post-loop store directly to label 99, so the
! loop body has an outgoing edge that is neither another loop block nor the
! construct exit. The wrap pass bails out and the CFG stays flat.
subroutine not_wrapped_outer_label(n, a)
  integer :: n, i
  real :: a(n)
  do i = 1, n
    if (a(i) > 0.0) goto 99
    a(i) = real(i)
  end do
  a(1) = -1.0
99 continue
end subroutine

! CHECK-LABEL: func.func @_QPnot_wrapped_outer_label
! CHECK-NOT:     scf.execute_region
! CHECK:         cf.cond_br
! CHECK:         return

! A plain, structured DO with no early exits: lowered as fir.do_loop,
! never reaches the wrap path (the loop is not unstructured at all).
subroutine structured(n, a)
  integer :: n, i
  real :: a(n)
  do i = 1, n
    a(i) = real(i)
  end do
end subroutine

! CHECK-LABEL: func.func @_QPstructured
! CHECK-NOT:     scf.execute_region
! CHECK:         fir.do_loop
! CHECK:         return

! Nested DOs whose only unstructuredness comes from a self-contained IF.
! Both DOs lower as fir.do_loop; only the IF is wrapped in scf.execute_region
! at the innermost level.
subroutine outer_structured_inner_wrapped(n, a)
  integer :: n, i, j
  real :: a(n)
  do i = 1, n
    do j = 1, n
      if (a(j) > 0.0) stop
      a(j) = real(i + j)
    end do
  end do
end subroutine

! CHECK-LABEL: func.func @_QPouter_structured_inner_wrapped
! CHECK:         fir.do_loop
! CHECK:           fir.do_loop
! CHECK:             scf.execute_region no_inline {
! CHECK:               cf.cond_br
! CHECK:               fir.call @_FortranAStopStatement
! CHECK:               fir.unreachable
! CHECK:               scf.yield
! CHECK:             }
! CHECK:             fir.result
! CHECK:           fir.result

! Structured outer DO containing an unstructured inner DO (the inner DO has
! an EXIT that targets its own construct exit). isUnstructured does not
! propagate from the inner DO to the outer DO, so the outer lowers as
! fir.do_loop. The inner DO's blocks would otherwise have nowhere to live
! inside the outer's single-block region, so the pre-wrap path creates an
! scf.execute_region around the inner DO's CFG.
subroutine outer_fir_do_loop_inner_unstructured_do(n, a)
  integer :: n, i, j
  real :: a(n)
  do i = 1, n
    do j = 1, n
      if (a(j) > 0.0) exit
    end do
  end do
end subroutine

! CHECK-LABEL: func.func @_QPouter_fir_do_loop_inner_unstructured_do
! CHECK:         fir.do_loop
! CHECK:           scf.execute_region no_inline {
! CHECK:             cf.br
! CHECK:             cf.cond_br
! CHECK:             cf.cond_br
! CHECK:             scf.yield
! CHECK:           }
! CHECK:           fir.result
