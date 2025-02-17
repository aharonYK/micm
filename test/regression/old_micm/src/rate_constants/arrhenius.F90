! Copyright (C) 2020 National Center for Atmospheric Research
! SPDX-License-Identifier: Apache-2.0
!
!> The rate_constant_arrhenius_t type
module micm_rate_constant_arrhenius

  use musica_constants,                only : musica_dk
  use micm_rate_constant,              only : rate_constant_t

  implicit none
  private

  public :: rate_constant_arrhenius_t

  !> An Arrhenius rate constant
  type, extends(rate_constant_t) :: rate_constant_arrhenius_t
    private
    real(kind=musica_dk) :: A_ = 1.0
    real(kind=musica_dk) :: B_ = 0.0
    real(kind=musica_dk) :: C_ = 0.0
    real(kind=musica_dk) :: D_ = 300.0
    real(kind=musica_dk) :: E_ = 0.0
  contains
    !> Returns the rate constant for a given set of conditions
    procedure :: calculate
    procedure :: output
  end type rate_constant_arrhenius_t

  interface rate_constant_arrhenius_t
    module procedure :: constructor
  end interface

contains

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  !> Constructor of Arrhenius rate constants
  elemental function constructor( A, B, C, D, E, Ea ) result( new_obj )

    use constants,                     only : kBoltzmann

    !> New rate constant
    type(rate_constant_arrhenius_t) :: new_obj
    !> Rate constant parameters
    real(kind=musica_dk), intent(in), optional :: A, B, C, D, E, Ea

    if( present( A  ) ) new_obj%A_ = A
    if( present( B  ) ) new_obj%B_ = B
    if( present( C  ) ) new_obj%C_ = C
    if( present( D  ) ) new_obj%D_ = D
    if( present( E  ) ) new_obj%E_ = E
    if( present( Ea ) ) new_obj%C_ = - Ea / kBoltzmann

  end function constructor

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  !> Returns the rate constant for a given set of conditions
  real(kind=musica_dk) elemental function calculate( this, environment )

    use micm_environment,              only : environment_t

    !> Reaction
    class(rate_constant_arrhenius_t), intent(in) :: this
    !> Environmental conditions
    type(environment_t), intent(in) :: environment

    calculate = this%A_ &
      * exp( this%C_ / environment%temperature ) &
      * ( environment%temperature / this%D_ ) ** this%B_ &
      * ( 1.0 + this%E_ *  environment%pressure )

  end function calculate

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  subroutine output(this)
    class(rate_constant_arrhenius_t), intent(in) :: this

    print *, this%A_
    print *, this%B_
    print *, this%C_
    print *, this%D_
    print *, this%E_
  end subroutine output

end module micm_rate_constant_arrhenius
