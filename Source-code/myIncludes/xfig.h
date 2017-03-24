#ifndef _XFIG_H_
#define _XFIG_H_

#include <deque>
#include <vector>
#include <string>

// As enumeracoes

typedef enum {
  XF_PORTRAIT=0, XF_LANDSCAPE=1
} XF_ORIENTATION;

typedef enum {
  XF_COLOR_DEFAULT=-1,
  XF_COLOR_BLACK=0, XF_COLOR_BLUE=1, XF_COLOR_GREEN=2,
  XF_COLOR_CYAN=3, XF_COLOR_RED=4,
  XF_COLOR_MAGENTA=5, XF_COLOR_YELLOW=6, XF_COLOR_WHITE=7,
  XF_COLOR_BLUE4=8, XF_COLOR_BLUE3=9, XF_COLOR_BLUE2=10, XF_COLOR_LIGHTBLUE=11,
  XF_COLOR_GREEN4=12, XF_COLOR_GREEN3=13, XF_COLOR_GREEN2=14,
  XF_COLOR_CYAN4=15, XF_COLOR_CYAN3=16, XF_COLOR_CYAN2=17,
  XF_COLOR_RED4=18, XF_COLOR_RED3=19, XF_COLOR_RED2=20,
  XF_COLOR_MAGENTA4=21, XF_COLOR_MAGENTA3=22, XF_COLOR_MAGENTA2=23,
  XF_COLOR_BROWN4=24, XF_COLOR_BROWN3=25, XF_COLOR_BROWN2=26,
  XF_COLOR_PINK4=27, XF_COLOR_PINK3=28, XF_COLOR_PINK2=29, XF_COLOR_PINK=30,
  XF_COLOR_GOLD=31
} XF_COLOR;

typedef enum {
  XF_NOFILL=-1,
  XF_FILL_DIAGLEFT30=41, XF_FILL_DIAGRIGHT30=42, XF_FILL_DIAGCROSS30=43,
  XF_FILL_DIAGLEFT45=44, XF_FILL_DIAGRIGHT45=45, XF_FILL_DIAGCROSS45=46,
  XF_FILL_HORIZBRICK=47, XF_FILL_VERTBRICK=48,
  XF_FILL_HORIZ=49, XF_FILL_VERT=50, XF_FILL_CROSS=51,
  XF_FILL_HORIZBRICKRIGHT=52, XF_FILL_HORIZBRICKLEFT=53,
  XF_FILL_VERTBRICKLEFT=54, XF_FILL_VERTBRICKRIGHT=55,
  XF_FILL_FISHSCALE2=56, XF_FILL_FISHSCALE1=57,
  XF_FILL_CIRCLE=58, XF_FILL_HEXAGON=59, XF_FILL_OCTAGON=60,
  XF_FILL_HORIZTIRE=61, XF_FILL_VERTTIRE=62
} XF_FILL_STYLE;

typedef enum {
  XF_LINE_SOLID=0, XF_LINE_DASH=1, XF_LINE_DOT=2,
  XF_LINE_DASHDOT=3, XF_LINE_DASH2DOT=4, XF_LINE_DASH3DOT=5
} XF_LINE_STYLE;

typedef enum {
  XF_JOIN_MITER=0, XF_JOIN_ROUND=1, XF_JOIN_BEVEL=2
} XF_JOIN_STYLE;

typedef enum {
  XF_CAP_BUTT=0, XF_CAP_ROUND=1, XF_CAP_PROJECT=2
} XF_CAP_STYLE;

typedef enum {
  XF_ARROW_OPEN=0, XF_ARROW_CLOSED=1,
  XF_ARROW_CLOSEDIN=2, XF_ARROW_CLOSEDOUT=3
} XF_ARROW_TYPE;

typedef enum {
  XF_ARROW_OUTLINE=0, XF_ARROW_SOLID=1
} XF_ARROW_STYLE;

typedef enum {
  XF_ARC_OPEN=1, XF_ARC_CLOSED=2
} XF_ARC_TYPE;

typedef enum {
  XF_ARC_CLOCK=1, XF_ARC_COUNTERCLOCK=0
} XF_ARC_DIRECTION;

typedef enum {
  XF_OVAL_ELLIPSRAD=1, XF_OVAL_ELLIPSDIAM=2,
  XF_OVAL_CIRCLERAD=3, XF_OVAL_CIRCLEDIAM=4
} XF_OVAL_TYPE;

typedef enum {
  XF_LINEAR_LINE=1, XF_LINEAR_BOX=2,
  XF_LINEAR_POLY=3, XF_LINEAR_ARCBOX=4,
  XF_LINEAR_FIGURE=5
} XF_LINEAR_TYPE;

typedef enum {
  XF_SPLINE_OPENAPP=0, XF_SPLINE_CLOSEDAPP=1,
  XF_SPLINE_OPENINT=2, XF_SPLINE_CLOSEDINT=3,
  XF_SPLINE_OPENXPL=4, XF_SPLINE_CLOSEDXPL=5,
} XF_SPLINE_TYPE;

typedef enum {
  XF_SPLINE_MIXED=0,
  XF_SPLINE_APPROX=1,
  XF_SPLINE_INTERP=2
} XF_SPLINE_STYLE;

typedef enum {
  XF_TEXT_LEFT=0, XF_TEXT_CENTER=1, XF_TEXT_RIGHT=2
} XF_TEXT_TYPE;

typedef enum {
  XF_TEXT_RIGID=1, XF_TEXT_SPECIAL=2, XF_TEXT_PS=4, XF_TEXT_HIDDEN=8
} XF_TEXT_FLAG;

// Nao criei o tipo enumerado para as fontes porque sao muitos tipos
// Quando tiver saco, eu faco. Por enquanto, as fontes sao descritas
// por um tipo inteiro cujo significado e dado a seguir

typedef int XF_TEXT_FONT;

/* FONTS

For font_flags bit 2 = 0 (LaTeX fonts):

  0 Default font
  1 Roman
  2 Bold
  3 Italic
  4 Sans Serif
  5 Typewriter

For font_flags bit 2 = 1 (PostScript fonts):

 -1 Default font
  0 Times Roman
  1 Times Italic
  2 Times Bold
  3 Times Bold Italic
  4 AvantGarde Book
  5 AvantGarde Book Oblique
  6 AvantGarde Demi
  7 AvantGarde Demi Oblique
  8 Bookman Light
  9 Bookman Light Italic
 10 Bookman Demi
 11 Bookman Demi Italic
 12 Courier
 13 Courier Oblique
 14 Courier Bold
 15 Courier Bold Oblique
 16 Helvetica
 17 Helvetica Oblique
 18 Helvetica Bold
 19 Helvetica Bold Oblique
 20 Helvetica Narrow
 21 Helvetica Narrow Oblique
 22 Helvetica Narrow Bold
 23 Helvetica Narrow Bold Oblique
 24 New Century Schoolbook Roman
 25 New Century Schoolbook Italic
 26 New Century Schoolbook Bold
 27 New Century Schoolbook Bold Italic
 28 Palatino Roman
 29 Palatino Italic
 30 Palatino Bold
 31 Palatino Bold Italic
 32 Symbol
 33 Zapf Chancery Medium Italic
 34 Zapf Dingbats

*/

// A classe base de todos os objetos

class XF_Object1
{
 private:
  // Profundidade do objeto
  unsigned depth;
  // A cor do objeto
  XF_COLOR color;
 public:
  XF_Object1(unsigned Depth=50);
  inline virtual ~XF_Object1() {}

  inline void setDepth(unsigned D) {depth=D;}
  inline void setColor(XF_COLOR LC) {
    color = LC;
  }
  virtual void setLine(unsigned LT, XF_LINE_STYLE LS,
		       float LV=3.0,
		       XF_CAP_STYLE CS=XF_CAP_BUTT,
		       XF_JOIN_STYLE JS=XF_JOIN_MITER);
  virtual void setFillPattern(XF_COLOR FC, XF_FILL_STYLE FS);
  // sat == 0.0         -> cor pura
  // 0.0 < sat <  100.0 -> cor clara
  // 0.0 > sat > -100.0 -> cor escura
  virtual void setFillColor(XF_COLOR FC, float sat=0.0);
  virtual void setArrowEnd(XF_ARROW_TYPE TY, XF_ARROW_STYLE ST,
			   unsigned TH, float WI, float HE);
  virtual void setArrowStart(XF_ARROW_TYPE TY, XF_ARROW_STYLE ST,
			     unsigned TH, float WI, float HE);
  virtual void setFont(XF_TEXT_FONT F, float Size);
  virtual void setOrientation(float Angle);
  virtual void setFlags(bool Rigid, bool Special,
			bool PostScript, bool Hidden);
  virtual int addLinearPoint(float X, float Y);
  virtual int addSplinePoint(float X, float Y, float Control=1000);

  friend std::ostream& operator<<(std::ostream &os, const XF_Object1 &xo);
  void saveDepth(FILE *file) const;
  void saveColor(FILE *file) const;
};

// As classes auxiliares representam os objetos auxiliares (aqueles
// que não fazem parte diretamente do XFig, mas são componentes dos
// objetos principais que integram o XFig)

class XF_Point
{
 public:
  float x,y;
  inline XF_Point(float X=0.0, float Y=0.0):
    x(X),y(Y) {}
  friend std::ostream& operator<<(std::ostream&, const XF_Point&);
  void save(FILE *file, float unit) const;
  void save(FILE *file, float unit, float maxY) const;
  void saveFloat(FILE *file, float unit, float maxY) const;
};

class XF_SPoint: public XF_Point
{
 public:
  float control;
  inline XF_SPoint(float X, float Y, float Control=0.0):
    XF_Point(X,Y),control(Control) {}
  inline XF_SPoint(XF_Point P, float Control=0.0):
    XF_Point(P),control(Control) {}
  friend std::ostream& operator<<(std::ostream&, const XF_SPoint&);
  void saveControl(FILE *) const;
};

class XF_Arrow
{
 private:
  bool present;
  XF_ARROW_TYPE type;
  XF_ARROW_STYLE style;
  unsigned thick;
  float width, height;
 public:
  XF_Arrow();
  inline bool Present() const {return present;}
  inline void set(XF_ARROW_TYPE TY, XF_ARROW_STYLE ST,
    unsigned TH, float WI, float HE) {
    present=true;
    type = TY;
    style = ST;
    thick = TH;
    width = WI;
    height = HE;
  }
  friend std::ostream& operator<<(std::ostream&, const XF_Arrow&);
  void saveData(FILE *file, float unit) const;
};

// Uma classe base para quase todos os objetos (exceto textos)

class XF_Object: public XF_Object1
{
 private:
  // A linha de desenho do objeto
  unsigned line_thick;
  XF_LINE_STYLE line_style;
  float line_style_val;
  XF_CAP_STYLE cap_style;
  XF_JOIN_STYLE join_style;
  // O preenchimento
  XF_COLOR fill_color;
  XF_FILL_STYLE fill_style;
  // As setas
  XF_Arrow forward, backward;
 public:
  XF_Object(unsigned Depth=50);
  inline virtual ~XF_Object() {}

  inline void setLine(unsigned LT, XF_LINE_STYLE LS,
        float LV=3.0,
        XF_CAP_STYLE CS=XF_CAP_BUTT,
        XF_JOIN_STYLE JS=XF_JOIN_MITER) {
    line_thick=LT;
    line_style=LS;
    line_style_val=LV;
    join_style=JS; 
    cap_style=CS;
  }
  inline void setFillPattern(XF_COLOR FC, XF_FILL_STYLE FS) {
    fill_color = FC;
    fill_style = FS;
  }
  // sat == 0.0         -> cor pura
  // 0.0 < sat <  100.0 -> cor clara
  // 0.0 > sat > -100.0 -> cor escura
  void setFillColor(XF_COLOR FC, float sat=0.0);
  inline void setArrowEnd(XF_ARROW_TYPE TY, XF_ARROW_STYLE ST,
     unsigned TH, float WI, float HE) {
    forward.set(TY,ST,TH,WI,HE);
  }
  inline void setArrowStart(XF_ARROW_TYPE TY, XF_ARROW_STYLE ST,
     unsigned TH, float WI, float HE) {
    backward.set(TY,ST,TH,WI,HE);
  }

  friend std::ostream& operator<<(std::ostream&, const XF_Object&);
  void saveCommon(FILE *) const;
  void saveCap(FILE *) const;
  void saveJoin(FILE *) const;
  void saveArrowsBool(FILE *file) const;
  void saveArrowsData(FILE *file, float unit) const;
};

// As classes que compoem o XFig

class XF_UserColor
{
 private:
  unsigned r,g,b;
 public:
  inline XF_UserColor(unsigned R, unsigned G, unsigned B):
    r(R),g(G),b(B) {}
  friend std::ostream& operator<<(std::ostream&, const XF_UserColor&);
  void save(FILE *, unsigned) const;
};

class XF_Arc: public XF_Object
{
 private:
  XF_ARC_TYPE subtype;
  XF_Point center;
  float radius;
  float theta1,theta2;
 public:
  inline XF_Arc(XF_ARC_TYPE T, XF_Point P, float R, float T1, float T2):
    XF_Object(),subtype(T),center(P),radius(R),theta1(T1),theta2(T2) {}
  float yMax(void) const;
  friend std::ostream& operator<<(std::ostream&, const XF_Arc&);
  void save(FILE *file, float unit, float maxY) const;
};

class XF_Oval: public XF_Object
{
 private:
  XF_OVAL_TYPE subtype;
  float angle;
  XF_Point center;
  XF_Point radius;
  XF_Point p1,p2;
 public:
  inline XF_Oval(XF_OVAL_TYPE T, float A,
   XF_Point C, XF_Point R, XF_Point P1, XF_Point P2):
    XF_Object(),subtype(T),angle(A),center(C),radius(R), p1(P1),p2(P2) {}
  float yMax(void) const;
  friend std::ostream& operator<<(std::ostream&, const XF_Oval&);
  void save(FILE *file, float unit, float maxY) const;
};

class XF_Linear: public XF_Object
{
 private:
  XF_LINEAR_TYPE subtype;
  unsigned radius;
  std::deque<XF_Point> pt;
  const XF_Point lastPoint() const;
 public:
  XF_Linear(XF_LINEAR_TYPE T, XF_Point P);
  XF_Linear(XF_LINEAR_TYPE T, XF_Point P1,
     XF_Point P2, unsigned Radius=0);
  XF_Linear(XF_LINEAR_TYPE T, XF_Point P1,
     XF_Point P2, XF_Point P3);
  float yMax(void) const;
  int addLinearPoint(float X, float Y);
  friend std::ostream& operator<<(std::ostream&, const XF_Linear&);
  void save(FILE *file, float unit, float maxY) const;
};

class XF_Figure: public XF_Object
{
 private:
  std::string figFile;
  bool rotated;
  std::vector<XF_Point> pt;
 public:
  XF_Figure(const char *File, XF_Point P1,
     XF_Point P2, bool Rotated);
  float yMax(void) const;
  friend std::ostream& operator<<(std::ostream&, const XF_Figure&);
  void save(FILE *file, float unit, float maxY) const;
};

class XF_Spline: public XF_Object
{
 private:
  XF_SPLINE_TYPE subtype;
  std::deque<XF_SPoint> spt;
 public:
  XF_Spline(XF_SPLINE_TYPE T, XF_Point P1,
     XF_Point P2);
  XF_Spline(XF_SPLINE_TYPE T, XF_Point P1,
     XF_Point P2, XF_Point P3, float Control);
  float yMax(void) const;
  XF_SPLINE_STYLE style() const;
  int addSplinePoint(float X, float Y, float Control);
  friend std::ostream& operator<<(std::ostream&, const XF_Spline&);
  void save(FILE *file, float unit, float maxY) const;
};

class XF_Text: public XF_Object1
{
 private:
  XF_TEXT_TYPE subtype;
  // A fonte
  XF_TEXT_FONT font;
  float size;
  // A inclinacao
  float angle;
  // As caracteristicas
  bool rigid,special,ps,hidden;
  // A posicao
  XF_Point pos;
  // As dimensoes
  float height,length;
  // O texto;
  std::string text;
  // Calcular as dimensoes
  void CalcDim();
 public:
  XF_Text(XF_TEXT_TYPE T, XF_Point P,
	  const char *Text);
  float yMax(void) const;
  void setFont(XF_TEXT_FONT F, float Size);
  void setOrientation(float Angle);
  void setFlags(bool Rigid, bool Special,
		bool PostScript, bool Hidden);
  friend std::ostream& operator<<(std::ostream&, const XF_Text&);
  void save(FILE *file, float unit, float maxY) const;
};
 
class XFig
{
private:
  float scale,unit;
  XF_ORIENTATION orient;
  bool upDown;
  std::deque<XF_UserColor> userColor;
  std::deque<XF_Arc> arc;
  std::deque<XF_Oval> oval;
  std::deque<XF_Linear> linear;
  std::deque<XF_Figure> figure;
  std::deque<XF_Spline> spline;
  std::deque<XF_Text> text;
public:
  XFig(XF_ORIENTATION orient=XF_PORTRAIT,
       bool UpDown=false);
  ~XFig();

  void setScale(float S);
  void setUnit(float U);
  // Para todos os metodos add... que retornam id, retorna -1 se erro

  // Adiciona uma cor do usuário; retorna o id (inteiro) desta cor (32,33,...)
  XF_COLOR addUserColor(unsigned R, unsigned G, unsigned B);
  // Adiciona um arco de circulo e retorna o id
  int addArc(XF_ARC_TYPE Type, float X, float Y, float Radius,
	     XF_ARC_DIRECTION Dir, float Theta1, float Theta2);
  // Adiciona um circulo ou elipse (OVAL) e retorna o id
  int addCircleRad(float X, float Y, float Radius);
  int addCircleDiam(float X1, float Y1,
		    float X2, float Y2);
  int addEllipseRad(float X, float Y,
		    float RadiusX, float RadiusY, float Angle=0.0);
  int addEllipseDiam(float X1, float Y1, float X2, float Y2);
  // Adiciona lineares (linha, poligono, etc) e retorna id
  // Adiciona uma linha e seu primeiro ponto
  int addLine(float X, float Y);
  // Adiciona um poligono e seus primeiros três pontos
  int addPolygon(float X1, float Y1,
		 float X2, float Y2,
		 float X3, float Y3);
  // Adiciona um quadrado (dois pontos)
  int addBox(float X1, float Y1,
      float X2, float Y2);
  int addRoundBox(float X1, float Y1,
		  float X2, float Y2, unsigned Rad);
  // Adiciona uma figura
  // O primeiro ponto eh aquele onde fica o pixel 0,0 da imagem
  // Se estiver no modo upDown (padrao imagem, xfig, etc),
  // o primeiro ponto deve ter as menores coordenadas x e y
  // Se estiver no padrao cartesiabo (eixo y downUp).
  // o primeiro ponto deve ter a menor coordenada x e maior y
  // Se fizer diferente, a imagem fica rotacionada
  int addFigure(const char*File, float X1, float Y1,
		float X2, float Y2, bool Rotated=false);
  // Adiciona splines e retorna id
  // Adiciona uma spline aberta e seus dois primeiros pontos
  int addSplineOpen(XF_SPLINE_STYLE Style, float X1, float Y1,
		    float X2, float Y2);
  // Adiciona uma spline fechada e seus três primeiros pontos
  int addSplineClosed(XF_SPLINE_STYLE Style, float X1, float Y1,
		      float X2, float Y2,
		      float X3, float Y3, float Control=1000);
  // Adiciona um texto
  int addText(XF_TEXT_TYPE T, float X, float Y,
	      const char *Text);
  // Acesso aos objetos (exceto aas cores do usuario)
  XF_Object1& operator[](int id);
  // Imprime na tela...
  friend std::ostream& operator<<(std::ostream&, const XFig&);
  // Salva o arquivo xfig: retorna true se erro
  bool save(const char*) const;
};

#endif
