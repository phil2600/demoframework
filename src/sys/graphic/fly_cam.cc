#include "../lib.hh"
#include "../includes/graphic.hh"

FreeFlyCamera::FreeFlyCamera(const Vector3D &position)
{
  position_ = position;
  phi_ = 0;
  theta_ = 0;
  VectorsFromAngles();

  speed_ = 0.01;
  sensivity_ = 0.2;
  verticalMotionActive_ = false;

  keyconf_["forward"] = SDLK_UP;
  keyconf_["backward"] = SDLK_DOWN;
  keyconf_["strafeleft_"] = SDLK_LEFT;
  keyconf_["strafe_right"] = SDLK_RIGHT;
  keyconf_["boost"] = SDLK_LSHIFT;

  _keystates[keyconf_["forward"]] = false;
  _keystates[keyconf_["backward"]] = false;
  _keystates[keyconf_["strafeleft_"]] = false;
  _keystates[keyconf_["strafe_right"]] = false;
  _keystates[keyconf_["boost"]] = false;

  SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_ShowCursor(SDL_DISABLE);
}


FreeFlyCamera::~FreeFlyCamera()
{
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(SDL_ENABLE);
}


void
FreeFlyCamera::OnMouseMotion(const SDL_MouseMotionEvent &event)
{
  theta_ -= event.xrel*sensivity_;
  phi_ -= event.yrel*sensivity_;
  VectorsFromAngles();
}


void
FreeFlyCamera::VectorsFromAngles()
{
  static const Vector3D up(0,0,1);

  if (phi_ > 89)
    phi_ = 89;
  else if (phi_ < -89)
    phi_ = -89;

  double r_temp = cos(phi_*M_PI/180);
  forward_.Z = sin(phi_*M_PI/180);
  forward_.X = r_temp*cos(theta_*M_PI/180);
  forward_.Y = r_temp*sin(theta_*M_PI/180);

  left_ = up.crossProduct(forward_);
  left_.normalize();

  target_ = position_ + forward_;
}


void
FreeFlyCamera::OnMouseButton(const SDL_MouseButtonEvent &event)
{
  if ((event.button == SDL_BUTTON_WHEELUP) && (event.type == SDL_MOUSEBUTTONDOWN)) //Wheelup
  {
    verticalMotionActive_ = true;
    timeBeforeStoppingVerticalMotion_ = 250; // 250ms
    verticalMotionDirection_ = 1; //up

  }
  else if ((event.button == SDL_BUTTON_WHEELDOWN)&&(event.type == SDL_MOUSEBUTTONDOWN))
  {
    verticalMotionActive_ = true;
    timeBeforeStoppingVerticalMotion_ = 250;
    verticalMotionDirection_ = -1;
  }
}


void
FreeFlyCamera::OnKeyboard(const SDL_KeyboardEvent &event)
{

  for (KeyStates::iterator it = _keystates.begin();it != _keystates.end();
       it++)
  {
    if (event.keysym.sym == it->first)
    {
      it->second = (event.type == SDL_KEYDOWN); //true on down, false on up
      break;
    }
  }
}


void
FreeFlyCamera::animate(Uint32 timestep)
{
  // Considering boost
  double realspeed = (_keystates[keyconf_["boost"]]) ? (10 * speed_) : speed_;

  if (_keystates[keyconf_["forward"]])
    position_ += forward_ * (realspeed * timestep);
  if (_keystates[keyconf_["backward"]])
    position_ -= forward_ * (realspeed * timestep);
  if (_keystates[keyconf_["strafeleft_"]])
    position_ += left_ * (realspeed * timestep);
  if (_keystates[keyconf_["strafe_right"]])
    position_ -= left_ * (realspeed * timestep);
  if (verticalMotionActive_)
  {
    if (timestep > timeBeforeStoppingVerticalMotion_)
      verticalMotionActive_ = false;
    else
      timeBeforeStoppingVerticalMotion_ -= timestep;
    position_ += Vector3D(0,0,verticalMotionDirection_*realspeed*timestep);
  }
  target_ = position_ + forward_;

}


void
FreeFlyCamera::look()
{
  gluLookAt(position_.X,position_.Y,position_.Z,
	    target_.X,target_.Y,target_.Z,
	    0,0,1);
}


void
FreeFlyCamera::setSpeed(double speed)
{
  speed_ = speed;
}


void
FreeFlyCamera::setSensivity(double sensivity)
{
  sensivity_ = sensivity;
}


void
FreeFlyCamera::setPosition(const Vector3D &position)
{
  position_ = position;
}


