#include "staticdo.h"
#include <QMessageBox>

StaticDO::StaticDO(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	//Set the minimum and close button of the main frame.
	this->setWindowFlags(Qt::WindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint));

    instantDoCtrl = InstantDoCtrl::Create();
	m_portPanel = NULL;
	portPanelLength = 0;

	this->layout = new QVBoxLayout(ui.scrollAreaWidgetContents);	

	images[0] = "background:url(:/StaticDO/Resources/ButtonUp.png)";
	images[1] = "background:url(:/StaticDO/Resources/ButtonDown.png)";
	images[2] = "background:url(:/StaticDO/Resources/ButtonDisabled.png)";

	connect(ui.btnConfig, SIGNAL(clicked()), this, SLOT(ButtonConfigureClicked()));
}

StaticDO::~StaticDO()
{

}

void StaticDO::Initialize() {
	//set the title of the form.
	setWindowTitle(tr("Static DO - Run(") + configure.deviceName + tr(")"));
    // 添加配置文件
	ConfigureDevice();
    // 根据配置文件初始化面板，数据IO的状态
	InitializePortPanel();
}

void StaticDO::ConfigureDevice() {
    std::wstring description = configure.deviceName.toStdWString();
    DeviceInformation selected(description.c_str());

	ErrorCode errorCode = Success;
	errorCode = instantDoCtrl->setSelectedDevice(selected);
	CheckError(errorCode);
    std::wstring profile = configure.profilePath.toStdWString();
    errorCode = instantDoCtrl->LoadProfile(profile.c_str());
    CheckError(errorCode);
	portCount = instantDoCtrl->getPortCount();
}

void StaticDO::InitializePortPanel() {
	//remove the old port panel
	if (m_portPanel != NULL) {
		int height = ui.scrollAreaWidgetContents->height();
		for (int i = 0; i < portPanelLength; i++) {
			this->layout->removeWidget(m_portPanel[i]);
			ui.scrollAreaWidgetContents->setGeometry(0, 0, 341, height - (i + 1) * 45);
			if (m_portPanel[i] != NULL) {
				delete m_portPanel[i];
				m_portPanel[i] = NULL;
			}
		}
	}

	DioFeatures * features = instantDoCtrl->getFeatures();
	Array<uint8>* portMasks = features->getDoDataMask();//getDataMask();

	quint8 *portStates = new quint8[portCount];
	ErrorCode errorCode = Success;
    errorCode = instantDoCtrl->Read(0, portCount, portStates);
    CheckError(errorCode);

	m_portPanel = new DioPortUI*[portCount];
    portPanelLength = portCount;
	for (int i = 0; i < portCount; i++) {
		ui.scrollAreaWidgetContents->setGeometry(0, 0, 341, (i + 1) * 45);
		m_portPanel[i] = new DioPortUI(0, 0, i, (quint8)0, images); 
		m_portPanel[i]->setMask(portMasks->getItem(i));
		m_portPanel[i]->setIsEditable(true);
		m_portPanel[i]->setState(portStates[i]);
		m_portPanel[i]->JudgeDirection(portMasks->getItem(i));

		connect(m_portPanel[i], SIGNAL(stateChanged(QObject*)), this, SLOT(stateChanged(QObject*)));

		this->layout->addWidget(m_portPanel[i]);
	}
}

void StaticDO::CheckError(ErrorCode errorCode) {
    if (BioFailed(errorCode))
	{
		QString message = tr("Sorry, there are some errors occurred, Error Code: 0x") +
			QString::number(errorCode, 16).right(8).toUpper();
		QMessageBox::information(this, "Warning Information", message);
	}
}

void StaticDO::ButtonConfigureClicked() {
	int dialogResult = configureDialog->exec();
	if (dialogResult == QDialog::Accepted) {
		configure = configureDialog->GetConfigureParameter();
		Initialize();
	}
}

void StaticDO::stateChanged(QObject* sender) {
    DioPortUI* panel = (DioPortUI*) sender;
    quint8 status =  panel->getState();
    ErrorCode errorCode = Success;
    errorCode = instantDoCtrl->Write(panel->getKey(), 1, &status);
    CheckError(errorCode);
}
