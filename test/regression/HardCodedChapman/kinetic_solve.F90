module kinetic_solve_mod
  use iso_c_binding

  implicit none

contains

  subroutine kinetic_solve(LU, b, x) bind(c)
    use iso_c_binding,      only : c_double
    use factor_solve_utilities, only : old_solver => solve

    real(kind=c_double), intent(in) :: LU(:), b(:)
    real(kind=c_double), pointer, intent(out) :: x(:)

    allocate(x(size(b)))

    x = 0

    call old_solver(LU, x, b)

  end subroutine kinetic_solve

end module kinetic_solve_mod