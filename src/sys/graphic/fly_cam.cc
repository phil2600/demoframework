#ifdef FREE_LOOK

# include "../lib.hh"
# include "../includes/graphic.hh"

FreeFlyCamera::FreeFlyCamera(const CPoint & position)
{
  _position = position;
  _phi = 0;
  _theta = 0;
  VectorsFromAngles();

  _speed = 0.05;
  _sensivity = 0.2;
  _verticalMotionActive = false;
  _keyconf["forward"] = SDLK_z;
  _keyconf["backward"] = SDLK_s;
  _keyconf["strafe_left"] = SDLK_q;
  _keyconf["strafe_right"] = SDLK_d;
  _keyconf["boost"] = SDLK_LSHIFT;
  _keystates[_keyconf["forward"]] = false;
  _keystates[_keyconf["backward"]] = false;
  _keystates[_keyconf["strafe_left"]] = false;
  _keystates[_keyconf["strafe_right"]] = false;
  _keystates[_keyconf["boost"]] = false;

  SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_ShowCursor(SDL_DISABLE);
}

FreeFlyCamera::~FreeFlyCamera()
{
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(SDL_ENABLE);
}

void
FreeFlyCamera::OnMouseMotion(const SDL_MouseMotionEvent & event)
{
  rotate(event.xrel, event.yrel);
}

void
FreeFlyCamera::OnMouseButton(const SDL_MouseButtonEvent & event)
{
  if ((event.button == SDL_BUTTON_WHEELUP)&&(event.type == SDL_MOUSEBUTTONDOWN)) //coup de molette vers le haut
  {
    _verticalMotionActive = true;
    _timeBeforeStoppingVerticalMotion = 250;
    _verticalMotionDirection = 1;

  }
  else if ((event.button == SDL_BUTTON_WHEELDOWN)&&(event.type == SDL_MOUSEBUTTONDOWN)) //coup de molette vers le bas
  {
    _verticalMotionActive = true;
    _timeBeforeStoppingVerticalMotion = 250;
    _verticalMotionDirection = -1;
  }
}

void
FreeFlyCamera::OnKeyboard(const SDL_KeyboardEvent & event)
{
  for (KeyStates::iterator it = _keystates.begin();it != _keystates.end();
       it++)
  {
    if (event.keysym.sym == it->first)
    {
      it->second = (event.type == SDL_KEYDOWN);
      break;
    }
  }
}

void
FreeFlyCamera::moveForward(Uint32 distance, double speed)
{
  _position += _forward * (speed * distance);
}

void
FreeFlyCamera::moveBackward(Uint32 distance, double speed)
{
  _position -= _forward * (speed * distance);
}

void
FreeFlyCamera::moveStrafeRight(Uint32 distance, double speed)
{
  _position -= _left * (speed * distance);
}

void
FreeFlyCamera::moveStrafeLeft(Uint32 distance, double speed)
{
  _position += _left * (speed * distance);
}

void
FreeFlyCamera::rotate(float x, float y)
{
  _theta -= x*_sensivity;
  _phi -= y*_sensivity;
  VectorsFromAngles();
}

void
FreeFlyCamera::animate(Uint32 timestep)
{
  double realspeed = (_keystates[_keyconf["boost"]])?10*_speed:_speed;
  if (_keystates[_keyconf["forward"]])
    moveForward(timestep, realspeed);
  if (_keystates[_keyconf["backward"]])
    moveBackward(timestep, realspeed);
  if (_keystates[_keyconf["strafe_left"]])
    moveStrafeLeft(timestep, realspeed);
  if (_keystates[_keyconf["strafe_right"]])
    moveStrafeRight(timestep, realspeed);
  if (_verticalMotionActive)
  {
    if (timestep > _timeBeforeStoppingVerticalMotion)
      _verticalMotionActive = false;
    else
      _timeBeforeStoppingVerticalMotion -= timestep;
    _position += CPoint(0,0,_verticalMotionDirection*realspeed*timestep); //scroll
  }
  recalc();
}

void
FreeFlyCamera::setSpeed(double speed)
{
  _speed = speed;
}

void
FreeFlyCamera::setSensivity(double sensivity)
{
  _sensivity = sensivity;
}

void
FreeFlyCamera::setPosition(const CPoint & position)
{
  _position = position;
  recalc();
}

void
FreeFlyCamera::VectorsFromAngles()
{
  static const CPoint up(0,0,1);
  if (_phi > 89)
    _phi = 89;
  else if (_phi < -89)
    _phi = -89;
  double r_temp = cos(_phi*M_PI/180);
  _forward.z = sin(_phi*M_PI/180);
  _forward.x = r_temp*cos(_theta*M_PI/180);
  _forward.y = r_temp*sin(_theta*M_PI/180);

  _left = up.crossProduct(_forward);
  _left.normalize();

  recalc();
}

void
FreeFlyCamera::look()
{
  gluLookAt(_position.x,_position.y,_position.z,
	    _target.x,_target.y,_target.z,
	    0,0,1);
}

void
FreeFlyCamera::recalc()
{
  _target = _position + _forward;
}

#endif /* FREE_LOOK */
