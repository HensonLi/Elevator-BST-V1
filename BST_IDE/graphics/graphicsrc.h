#ifndef GRAPHICSRC_H
#define GRAPHICSRC_H

#include "global.h"
#include <QTime>
#include <QVector>
#include <QDir>
#include <QKeyEvent>

#ifdef  SURRPORT_3D
#include <QtOpenGL>
#include <QGLWidget>
#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtOpenGL/qglshaderprogram.h>
#include "./OpenGL/Windows/include/glut.h"
#endif

class GraphicsScene;

#ifdef  SURRPORT_3D
class GraphicsGLWidget;
#endif

//>@pRect��ʾ���ǵ���ͨ��Element���г�ʼ��������ߵ�ǰui�ļ���û�д˿ؼ�ʱ��ͨ����rect���г�ʼ��ui�еĽڵ��Լ��������λ�á�
class GraphicsRC
{
public:
    GraphicsRC(GraphicsScene *pScene, QRectF pRect);
    ~GraphicsRC();

    int GetKey(QString pString);

    QPixmap LoadPixmap(QString pPixmapName, QSizeF pSize=QSizeF());
    QPixmap ScalePixmap(QPixmap pPixmap, QSizeF pSize=QSizeF());

    QPixmap GetPixmap(int pKey, QSizeF pSize=QSizeF());  //>@��m_EffectXXX�м���ͼƬ
#ifdef  SURRPORT_SVG
    QSvgRenderer *GetSvgRender(int pKey);
#endif

    void ReleaseEffectGroup();
    void ReleasePixmaps();
    void ReleaseMovies();
    void ReleaseTextures() {}
    void ReleaseSvgRender();

    void ReleaseRC();
    bool RefreshRC();

    bool LoadPath(int pKey, QString pPixmapName);

    int getNextRc(int pCurRcIndex);
    int getPrevRc(int pCurRcIndex);

    void InitScene(GraphicsScene* pScene);

    void DeleteComponent();  //>@��UI�ļ���ɾ���˽ڵ������
    bool InitComponent(QHash<int, QPixmap> pPixmapList, QHash<AREA_OPERATE, AREA_ANIMATE*>  pEffectGroup);
    bool InitComponent(QDomElement &pElement, bool pOffset = false);
    bool InitState(QDomElement pElement);

    bool InitSubPara(QString pStateName, QString pLabel, QString pContent);

    bool InitCommonPara(AREA_STYLE* pAreaStyle, QString pCommonPara);
    virtual bool InitSubCommonPara(AREA_STYLE* pAreaStyle, QString pLabel, QString pContent);

    bool InitEffectPara(AREA_ANIMATE* pAreaEffect, QString pEffectPara);
    virtual bool InitSubEffectPara(AREA_ANIMATE* pAreaEffect, QString pLabel, QString pContent);

    virtual bool Start() {return true;}  //>@ִ����InitEffect��Ҫִ��Start��������Ч

    QList<EFFECT*> GetDefaultEffects(QString pStateName);
    EFFECT* GetDefaultEffect(QList<EFFECT*> pEffects, QString pEffectName);
    EFFECT* GetDefaultEffect(QString pStateName, QString pEffectName);
    QDomElement GetComStateElement(QString pStateName, QString &pEffectName, QString &pEffectPara);

    QString GetComStateEffect(QString pStateName);
    QString GetComStateEffectPara(QString pStateName, QString pParaName);
    QString GetComStateRcFile(QString pStateName, QString pRcName);

public:
#ifdef IDE
    ComCategory                        *m_ComCategory;
#endif
    GraphicsScene                      *m_UiScene;
    QDomElement                         m_ComElement;
    COM_TYPE                            m_ComType;
    QString                             m_ComPath;
    QRectF                              m_ComRect;

    QList<int>                          m_RcList;       //>@���ڲ�����һ������һ����Դ

    QHash<int, QString>                 m_EffectPath;   //>@ֻ��Ҫ��¼·������Դ
    QHash<int, QPixmap>                 m_EffectImage;  //>@���ص��ڴ����Դ
    QHash<int, QMovie*>                 m_EffectMovie;  //>@
#ifdef  SURRPORT_3D
    QHash<int, GLuint>                  m_EffectTexture;
#endif
#ifdef  SURRPORT_SVG
    QHash<int, QSvgRenderer*>           m_EffectSvgRender;
#endif
    AREA_STYLE*                         m_RcStyle;  //>@��Ч����
    QHash<AREA_OPERATE, AREA_ANIMATE*>  m_EffectGroup;  //>@��Ч����
    bool                                m_EffectValid;  //>@��Чʹ��
};

#endif // GRAPHICSRC_H
