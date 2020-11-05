#include "staticdi.h"
#include <QMessageBox>

StaticDI::StaticDI(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
    ui.setupUi(this);
    //Set the minimum and close button of the main frame.
    this->setWindowFlags(Qt::WindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint));

    instantDiCtrl = InstantDiCtrl::Create();
    m_portPanel = NULL;
    portPanelLength = 0;
    timer = new QTimer(this);

    this->layout = new QVBoxLayout(ui.scrollAreaWidgetContents);

    images[0] = "background:url(:/StaticDI/Resources/ledLow.png)";
    images[1] = "background:url(:/StaticDI/Resources/ledHigh.png)";

    connect(ui.btnConfig, SIGNAL(clicked()), this, SLOT(ButtonConfigureClicked()));
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerTicked()));
}

StaticDI::~StaticDI()
{

}

void StaticDI::Initialize() {
	//set the title of the form.
	setWindowTitle(tr("Static DI - Run(") + configure.deviceName + tr(")"));

	ConfigureDevice();
	InitializePortPanel();

	//start the timer to read DI ports status
	timer->start(50);
}

void StaticDI::ConfigureDevice() {
    std::wstring description = configure.deviceName.toStdWString();
    DeviceInformation selected(description.c_str());

	ErrorCode errorCode = Success;
	errorCode = instantDiCtrl->setSelectedDevice(selected);
	CheckError(errorCode);

    std::wstring profile = configure.profilePath.toStdWString();
    errorCode = instantDiCtrl->LoadProfile(profile.c_str());
	CheckError(errorCode);
	portCount = instantDiCtrl->getPortCount();
}

void StaticDI::InitializePortPanel() {
	//remove the old port panel
	if (m_portPanel != NULL) {
		int height = ui.scrollAreaWidgetContents->height();
		for (int i = 0; i < portPanelLength; i++) {
			this->layout->removeWidget(m_portPanel[i]);
			ui.scrollAreaWidgetContents->setGeometry(0, 0, 341, height - (i + 1) * 50);
			if (m_portPanel[i] != NULL) {
				delete m_portPanel[i];
				m_portPanel[i] = NULL;
			}
		}
	}

	m_portPanel = new DioPortUI*[portCount];
    portPanelLength = portCount;
	for (int i = 0; i < portCount; i++) {
		ui.scrollAreaWidgetContents->setGeometry(0, 0, 341, (i + 1) * 50);
		m_portPanel[i] = new DioPortUI(0, 0, i, (quint8)0, images); 
		m_portPanel[i]->setIsEditable(false);

		this->layout->addWidget(m_portPanel[i]);
	}
}

void StaticDI::CheckError(ErrorCode errorCode) {
    if (BioFailed(errorCode))
	{
		QString message = tr("Sorry, there are some errors occurred, Error Code: 0x") +
			QString::number(errorCode, 16).right(8).toUpper();
		QMessageBox::information(this, "Warning Information", message);
	}
}

void StaticDI::ButtonConfigureClicked() {
	timer->stop();

	int dialogResult = configureDialog->exec();
	if (dialogResult == QDialog::Accepted) {
		configure = configureDialog->GetConfigureParameter();
		Initialize();
	}
}

void StaticDI::TimerTicked() {
	//read DI data
    quint8 *portStates = new quint8[portCount];
    ErrorCode errorCode = Success;
    errorCode = instantDiCtrl->Read(0, portCount, portStates);
    CheckError(errorCode);

	//update the UI
	for (int i = 0; i < portCount; i++) {
		this->m_portPanel[i]->setState(portStates[i]);
	}
}
