# include "../lib.hh"
# include "../includes/graphic.hh"

Camera::Camera(const CPoint & position)
{
  _position = position;
  _phi = 0;
  _theta = 0;
  VectorsFromAngles();

  _speed = 0.05;
  _sensivity = 0.2;
  _verticalMotionActive = false;

  keys_.forward.state = false;
  keys_.backward.state = false;
  keys_.strafe_left.state = false;
  keys_.strafe_right.state = false;
  keys_.boost.state = false;

  SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_ShowCursor(SDL_DISABLE);
}

Camera::~Camera()
{
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(SDL_ENABLE);
}

void
Camera::OnMouseMotion(const SDL_MouseMotionEvent & event)
{
  rotate(event.xrel, event.yrel);
}

void
Camera::OnMouseButton(const SDL_MouseButtonEvent & event)
{
  if ((event.button == SDL_BUTTON_WHEELUP)&&(event.type == SDL_MOUSEBUTTONDOWN)) //coup de molette vers le haut
  {
    _verticalMotionActive = true;
    _timeBeforeStoppingVerticalMotion = 250;
    _verticalMotionDirection = -1;

  }
  else if ((event.button == SDL_BUTTON_WHEELDOWN)&&(event.type == SDL_MOUSEBUTTONDOWN)) //coup de molette vers le bas
  {
    _verticalMotionActive = true;
    _timeBeforeStoppingVerticalMotion = 250;
    _verticalMotionDirection = 1;
  }
}

void
Camera::OnKeyboard(const SDL_KeyboardEvent & event)
{
  switch (event.keysym.sym)
  {
    case SDLK_z:
      keys_.forward.state = (event.type == SDL_KEYDOWN);
      break;
    case SDLK_s:
      keys_.backward.state = (event.type == SDL_KEYDOWN);
      break;
    case SDLK_q:
      keys_.strafe_left.state = (event.type == SDL_KEYDOWN);
      break;
    case SDLK_d:
      keys_.strafe_right.state = (event.type == SDL_KEYDOWN);
      break;
    case SDLK_LSHIFT:
      keys_.boost.state = (event.type == SDL_KEYDOWN);
      break;
  }
}

void
Camera::moveForward(Uint32 distance, double speed)
{
  _position += _forward * (speed * distance);
}

void
Camera::moveBackward(Uint32 distance, double speed)
{
  _position -= _forward * (speed * distance);
}

void
Camera::moveStrafeRight(Uint32 distance, double speed)
{
  _position -= _left * (speed * distance);
}

void
Camera::moveStrafeLeft(Uint32 distance, double speed)
{
  _position += _left * (speed * distance);
}

void
Camera::rotate(float x, float y)
{
  _theta -= x*_sensivity;
  _phi -= y*_sensivity;
  VectorsFromAngles();
}

void
Camera::animate(Uint32 timestep)
{
  double realspeed = keys_.boost.state ? 10*_speed : _speed;
  if (keys_.forward.state)
    moveForward(timestep, realspeed);
  if (keys_.backward.state)
    moveBackward(timestep, realspeed);
  if (keys_.strafe_left.state)
    moveStrafeLeft(timestep, realspeed);
  if (keys_.strafe_right.state)
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
Camera::setSpeed(double speed)
{
  _speed = speed;
}

void
Camera::setSensivity(double sensivity)
{
  _sensivity = sensivity;
}

void
Camera::setPosition(const CPoint & position)
{
  _position = position;
  recalc();
}

void
Camera::VectorsFromAngles()
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
Camera::look()
{
  gluLookAt(_position.x, _position.y, _position.z,
	    _target.x, _target.y, _target.z,
	    0,0,1);
}

void
Camera::recalc()
{
  _target = _position + _forward;
}
