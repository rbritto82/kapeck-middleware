/***************************************************************************
 *            funcoes.h
 *
 *  Mon Mar 19 14:36:58 2007
 *  Copyright  2007  User Ricardo Britto	
 *  Email nephlinvocals@dca.ufrn.br
 ****************************************************************************/

#ifndef _FUNCOES_H
#define _FUNCOES_H

#include <math.h>

// Um arco tangente que funciona para ângulos de +- 90 graus
static inline double arc_tang(double y, double x)
{
  return (x != 0.0 ? atan2(y,x) : (y>=0 ? M_PI/2.0 : -M_PI/2.0));
}

// Retorna um ângulo equivalente no intervalo entre -180 graus e +180 graus
static inline double ang_equiv(double theta)
{
  return (arc_tang(sin(theta), cos(theta)));
}

// Retorna um ângulo equivalente no intervalo entre -90 graus e +90 graus
static inline double ang_equiv2(double theta)
{
  if (cos(theta)==0.0) {
    if (sin(theta)>=0.0) return M_PI_2;
    else return -M_PI_2;
  }
  else return (atan(tan(theta)));
}

// Eleva ao quadrado
static inline double pow2(double x)
{
  return (x*x);
}

// Retornam o máximo e mínimo
static inline double max(double x, double y) {
  return (x>y ? x : y);
}
static inline double min(double x, double y) {
  return (x<y ? x : y);
}

#endif /* _FUNCOES_H */
