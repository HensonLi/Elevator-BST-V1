/---------------------------------------------------------------
| VERSION 1.0.0.1 ��һ�������
| VERSION 1.0.0.2 ���ӵ�һ�͵ڶ���UI�������
| VERSION 1.0.0.3 ���ӵ�����UI��ƺ͵�һ��ģ�������
| VERSION 1.0.0.4 ���ӵڶ���ģ�������
---------------------------------------------------------------/

1��js�ļ��У�
    LineEdit:.text
    CheckBox:.checked
    Combobox:.currentText
2��  /*QDir::currentPath()*/
3��FloorBtn������13(��ť�ܸ���);5x3(����);2(��ť�м��);1(��ť�м��);1000(��Чʱ��);
4��windows���ã�ֻҪ��GlobalFun.h���޸�WINDOWS�궨�弴�ɡ�
   UBUNTU���ã�1���޸ĺ궨��
              2����Ҫ��Сд���⣬���硶Phonon/Meobject����ubunut����Ҫд�ɡ�phonon/meobject��
              3��ȥ��USB.pri
              4��
5������������в��ɹ����������dll�ļ�û�з���system32�е�ԭ����usb�ļ����С�
6������������ķ�����
    1���½�effectxxx�࣬����effectinc��include
    2����globalfun.h�ļ��е�COM_TYPE�������µ��������,��getComponetType��getComPath����Ӵ˿ؼ�����
    3����boot.xml�ļ������Ӵ�������������subui�����ӣ�
    4����global.h�ļ�AddComDom���������
    5����MainWindow.h-134��������¿ؼ����ͣ���MainWindow.cpp�е�AddTools/CreateRightFrame��������Ӵ���
    6��ManageWidgetItem::start����Ӵ���
    7����DiagramScene::OperateComDiagram�е�update�����Ӵ���
    8����simulatorform����InitUI()����Ӵ���
    9����digramcom�е�ComBaseWidget::InitEffect�����Ӵ���
7�������������ò����ķ�����
    1����diagramsetting.cpp�е�EffectSetting::accept()�����
    2����diagramsetting.cpp�е�EffectSetting::CreateUi()�����
8����¼�������˵��
   �������������������������и���̨�����ʹ�������������ͬ������Э�飬��Ҫ��ִ�пͻ���Ҫ��Ĳ�ѯ���ļ�����������Ƿ����ӣ����Ӻ����������ؾ���
   ������Ӱ�ť���������Եذ��ϵ磬��������¿�������uboot����ʼ��������¼�ں˲�������ʱ10s��û�н������������������ʾ��
9��Log4QTʹ�ã�
    ��������IDE_Log.conf�ļ����޸�log4j.appender.E.File��Ϊ����·���µ�RuntimeRc/Log/BST_IDE.log��
    ע�⣬��Windows������·��һ��Ϊ.../IDE/BST_IDE/��������.../IDE/BST_IDE/debug��Ҫ�ڡ���Ŀ��->�����С����޸ġ�
    Ȼ��ʹ��GetLoggerInfo������ʼ��Log·���Լ�ȫ���࣬���Ϳ���ʹ��Loggerд��־�ˡ�

    IDE_Log.conf�Ĳ��Ҵ�����·�����𣬶������й�.log�ļ���·���ӳ���·����ʼ��

10�����޸�
     Text�������HV�����趨���ű�������ߵȱ������ţ���Ȼ�������Ч����
     clock����ʱ���������Ӽ��̶ֹ�λ����Ч��ģ��ʱ�ӽ����ת�Ƕ�ƫ��270����
     Ϊtreewidget�̳�����
11������Demon��Display�Ļ�����
    windows��
        1.��D:\MyProject\IDE\BST_DAEMON\debug\RuntimeRcĿ¼������FSRC������TFlash0������USB0�������ļ��У�����TFlash0�з����������
        2.
12���½�һ������QHash��KEY����ʱ��Ҫ���==�����������Լ����������
    uint qHash(const NEWCLASS &key)
    {
        return (key.mPInterface << 16) + key.mLDevice; //>@ֻҪ���Ի��Ψһ�����ݼ��ɡ�
    }
13��NFD�ļ�û�и��¡�
    Global����Mdp��û�и��¡�

1��log����
2��

#SUBDIRS +=  libQtAV
#BST_IDE.depends += libQtAV
#libQtAV.file = QtAV/QtAV.pro
#include(QtAV/root.pri)
