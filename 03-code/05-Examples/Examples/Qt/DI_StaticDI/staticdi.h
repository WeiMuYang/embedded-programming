#ifndef STATICDI_H
#define STATICDI_H

#include <QDialog>
#include <QTimer>
#include "ui_staticdi.h"
#include "configuredialog.h"
#include "../common/DioPortUI.h"

class StaticDI : public QDialog
{
	Q_OBJECT

public:
	StaticDI(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~StaticDI();

	void Initialize();
	void InitializePortPanel();
	void SetConfigureDialog(ConfigureDialog * dialog){configureDialog = dialog;}
	void SetConfigureParameter(ConfigureParameter value){configure = value;}
	void ConfigureDevice();
	void CheckError(ErrorCode errorCode);

private:
	Ui::StaticDIClass ui;
	ConfigureDialog *configureDialog;
	ConfigureParameter configure;
	InstantDiCtrl* instantDiCtrl;
	DioPortUI** m_portPanel;
	QVBoxLayout* layout;

	int portCount;
	int portPanelLength;
	QString images[2];
	QTimer *timer;

private slots:
	void ButtonConfigureClicked();
	void TimerTicked();
};

#endif // STATICDI_H
