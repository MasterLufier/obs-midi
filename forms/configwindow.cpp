#include <inttypes.h>


#include <obs.hpp>
#include <util/platform.h>

#include "configwindow.h"
#include "ui_configwindow.h"
#include <QtWidgets>
#include <QTableWidget>
#include <obs-frontend-api/obs-frontend-api.h>
#include "obs-midi.h"
#include "config.h"
#include "device-manager.h"
#include "midi-agent.h"

ConfigWindow::ConfigWindow( std::string devicename)
	
{
	//MakeSceneCombo();
	
	//auto rob = static_cast<RouterPtr>(GetRouter());
	auto devicemanager = GetDeviceManager();
	auto config = GetConfig();
	//config->Load();
	Router *rt = midiobsrouter;	
	//connect(this, SIGNAL(&SendNewUnknownMessage), this	SIGNAL(rt.UnknownMessage));
	auto device = devicemanager->GetMidiDeviceByName(devicename.c_str());

	std::vector<MidiHook *> hooks =	devicemanager->GetMidiHooksByDeviceName(devicename.c_str());
		
	connect( rt, SIGNAL(UnknownMessage(QString, int)), this,SLOT(domessage(QString, int)));
	//Setup the UI
	ui.setupUi(this);


//connect
	//Connect back button functionality
	connect(ui.btnBack, &QPushButton::clicked, this,&ConfigWindow::on_btn_back_clicked);
	//connect(ui.btnSave, &QPushButton::clicked, this, &ConfigWindow::save);

	
		for (int i = 0; i < hooks.size(); i++) {
		int rc = ui.tableWidget->rowCount();
		AddRowFromHooks(rc,hooks.at(i)->type, hooks.at(i)->index, false,
				hooks.at(i)->action, hooks.at(i)->command,
				hooks.at(i)->param1, hooks.at(i)->param2,
				hooks.at(i)->param3);
	}
	
	
	
	//setup model
	ConfigWindow::SetupModel();
	//create default actions
	ConfigWindow::chooseAtype(1);
	//create model
	//ConfigWindow configTableModel;

	//Wrap Data into Model
		
	
	
	

	

	connect(ui.cb_atype, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseAtype(int)));
	//connect(ui.cb_action, SIGNAL(currentTextChanged(QString)), configTableModel, SLOT(chooseOptions1(QString)));

	
	
}

void ConfigWindow::AddRowFromHooks(int rc, std::string type, int index, bool bid,
			     std::string action, std::string command,
			     std::string param1, std::string param2,
			     std::string param3)
{

		ui.tableWidget->insertRow(rc);
		QTableWidgetItem *newItem = new QTableWidgetItem();
		QTableWidgetItem *newItem2 = new QTableWidgetItem();
		QTableWidgetItem *newItem3 = new QTableWidgetItem();
		QTableWidgetItem *newItem4 = new QTableWidgetItem();
		QTableWidgetItem *newItem5 = new QTableWidgetItem();
		QTableWidgetItem *newItem6 = new QTableWidgetItem();
		QTableWidgetItem *newItem7 = new QTableWidgetItem();
		QTableWidgetItem *newItem8 = new QTableWidgetItem();

		
		//load rows from hooks
		//bidirectional.append(false);

		
		newItem->setText(QString::fromStdString(type)); //Message Type
		newItem2->setText(QString::number(index)); //message channel
		//newItem3->setCheckState(false); //Bidirectional
		newItem4->setText(QString::fromStdString(action)); //Action Type
		newItem5->setText(QString::fromStdString(command)); //Action
		newItem6->setText(QString::fromStdString(param1)); //Option 1
		newItem7->setText(QString::fromStdString(param2)); //Option 2
		newItem8->setText(QString::fromStdString(param3));        //Option 3

		//if (!tm_messagenumber.contains(mindex)) {

		ui.tableWidget->setItem(rc, 0, newItem);
		ui.tableWidget->setItem(rc, 1, newItem2);
		ui.tableWidget->setItem(rc, 2, newItem3);
		ui.tableWidget->setItem(rc, 3, newItem4);
		ui.tableWidget->setItem(rc, 4, newItem5);
		ui.tableWidget->setItem(rc, 5, newItem6);
		ui.tableWidget->setItem(rc, 6, newItem7);
		ui.tableWidget->setItem(rc, 7, newItem8);	
}
void  ConfigWindow::insertRow(QString mtype,int mindex)
{

	int rc = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(rc );
	QTableWidgetItem *newItem = new QTableWidgetItem();
	QTableWidgetItem *newItem2 = new QTableWidgetItem();
	QTableWidgetItem *newItem3 = new QTableWidgetItem();
	QTableWidgetItem *newItem4 = new QTableWidgetItem();
	QTableWidgetItem *newItem5 = new QTableWidgetItem();
	QTableWidgetItem *newItem6 = new QTableWidgetItem();
	QTableWidgetItem *newItem7 = new QTableWidgetItem();
	QTableWidgetItem *newItem8 = new QTableWidgetItem();
	
	newItem->setText(mtype); //Message Type
	newItem2->setText(QString::number(mindex));   //message channel
	//newItem3->setCheckState(false); //Bidirectional
	
	if (mtype == "control_change") {
		newItem4->setText("Fader"); //Action Type
		newItem5->setText("Set Volume"); //Action	tm_actiontype.append("Fader");
		
	} else {
		newItem4->setText("Button");  //Action Type
		newItem5->setText("Set Mute"); //Action
		
	}
	newItem6->setText("Mic/Aux");   //Option 1
	newItem7->setText("");   //Option 2
	newItem8->setText("");   //Option 3

	
	
	//if (!tm_messagenumber.contains(mindex)) {
	
	
	ui.tableWidget->setItem(rc , 0, newItem);
	ui.tableWidget->setItem(rc, 1, newItem2);
	ui.tableWidget->setItem(rc, 2, newItem3);
	ui.tableWidget->setItem(rc, 3, newItem4);
	ui.tableWidget->setItem(rc, 4, newItem5);
	ui.tableWidget->setItem(rc, 5, newItem6);
	ui.tableWidget->setItem(rc, 6, newItem7);
	ui.tableWidget->setItem(rc, 7, newItem8);

	
}
void ConfigWindow::save(QString devicename) {
	//Get Device Manager
	auto dm = GetDeviceManager();
	auto save = GetConfig();
	//to get device
	auto dev = dm->GetMidiDeviceByName(devicename.toStdString().c_str());
	dev->ClearMidiHooks();
	//get row count
	int rc =ui.tableWidget->rowCount();
	//loop through rows
	for (int i=0; i < rc; i++)
	{
	//make default midihook
		
		MidiHook *mh = new MidiHook;
		
		//map values
		mh->type = ui.tableWidget->item(i, 0)->text().toStdString();
		mh->index =ui.tableWidget->item(i, 1)->text().toInt();
		//mh->bidirectional = ui.tableWidget->item(i, 2)->text;
		mh->action = ui.tableWidget->item(i, 3)->text().toStdString();
		mh->command = ui.tableWidget->item(i, 4)->text().toStdString();
		mh->param1 = ui.tableWidget->item(i, 5)->text().toStdString();
		mh->param2 = ui.tableWidget->item(i, 6)->text().toStdString();
		mh->param3 = ui.tableWidget->item(i, 7)->text().toStdString();
		
		dev->AddMidiHook(mh);
		
	};
	save->Save();
}
	void ConfigWindow::domessage(QString mtype, int mchan)
{
	
	blog(1, "domessage");
	if (!inrow(mchan)) {
		if (!inrow(mtype)) {
			insertRow(mtype, mchan);
		}
		
	}
}
bool ConfigWindow::inrow(int x) {

int rows = ui.tableWidget->rowCount();
	bool found = false;
	for (int i = 0; i < rows; ++i) {
		if (ui.tableWidget->item(i, 1)->text() ==
		    QString::number(x)) {
			return true;
		}
	}
	return false;
}
bool ConfigWindow::inrow(QString mtype)
{

	int rows = ui.tableWidget->rowCount();
	bool found = false;
	for (int i = 0; i < rows; ++i) {
		if (ui.tableWidget->item(i, 1)->text() == mtype) {
			return true;
		}
	}
	return false;
}
void ConfigWindow::rebuildModel() {}





void ConfigWindow::SetupModel()
{
	//Make models for combo box and add to combobox
	QStringList items;
	items << tr("Button") << tr("Fader") ;
	actiontypemodel = new QStringListModel(items, this);
	actiontypemodel->setProperty("role",2);
	ui.cb_atype->setModel(actiontypemodel);
}




	// Create a method to populate the model with data:


void ConfigWindow::on_btn_back_clicked()
{

	setVisible(false);
	//&parentWidget->show()
	//	parent::show();
}

ConfigWindow::~ConfigWindow()
{
	
}
void ConfigWindow::ToggleShowHide()
{
	if (!isVisible())
		setVisible(true);
	else
		setVisible(false);
}
void ConfigWindow::load(){};
void ConfigWindow::addrow(){};
void ConfigWindow::deleterow(){};
void ConfigWindow::updateUi(){};
void ConfigWindow::selectionChanged(){};
void ConfigWindow::loadFromHooks()
{
	


}
/*bool ConfigWindow::insertRow(int row, std::string  mtype, int mindex, std::string actiontype,
		std::string action, std::string option1, std::string option2,
		std::string option3, const QModelIndex &parent)*/



/*                Make Combo list models
*/
void ConfigWindow::chooseOptions1(QString Action) {
	QList<QString> option1;
	QList<QString> option2;
	QList<QString> volume;
	QStringList nada;
	if (Action == "SetVolume") {
		
		//configTableModel->PopulateOptions(volumeModel, nada, nada);
		//configTableModel->options1model->
		
	} else if (Action == "SetCurrentScene") {
		
	}
}
/*
tm_option2.clear();
tm_option2 = option2;
tm_option3.clear();
tm_option3 = option3;
*/
QStringList ConfigWindow::MakeSceneCombo()
{
	QStringList opitems;
	auto scenes = Utils::GetScenes();
	auto length = obs_data_array_count(scenes);
	for (size_t i = 0; i < length; i++) {
		auto d = obs_data_array_item(scenes, i);
		auto name = obs_data_get_string(d, "name");
		opitems << tr(name);
	}
	return opitems;
	//ui.cb_param1->setModel(options1model);
}


QStringList ConfigWindow::MakeVolumeCombo()
{
	QStringList opitems;
	QMap<const char *, int> sources;
	sources["desktop-1"] = 1;
	sources["desktop-2"] = 2;
	sources["mic-1"] = 3;
	sources["mic-2"] = 4;
	sources["mic-3"] = 5;

	QMapIterator<const char *, int> i(sources);
	while (i.hasNext()) {
		i.next();

		const char *id = i.key();
		OBSSourceAutoRelease source = obs_get_output_source(i.value());
		if (source) {
			opitems << obs_source_get_name(source);
		}
	}
	return opitems;
	
}
// Choose Action Type Handler
QStringList ConfigWindow::chooseAtype(int index)
{
	//actiontypemodel = new QStringListModel(items, this);
	QStringList items;
	switch (index) {
	case 0:
		//Button

		items << tr("SetCurrentScene") << tr("SetPreviewScene")
		      << tr("TransitionToProgram") << tr("SetCurrentTransition")
		      << tr("SetSourceVisibility")
		      << tr("ToggleSourceVisibility") << tr("ToggleMute")
		      << tr("SetMute") << tr("StartStopStreaming")
		      << tr("StartStreaming") << tr("StopStreaming")
		      << tr("StartStopRecording") << tr("StartRecording")
		      << tr("StopRecording") << tr("StartStopReplayBuffer")
		      << tr("StartReplayBuffer") << tr("StopReplayBuffer")
		      << tr("SaveReplayBuffer") << tr("PauseRecording")
		      << tr("ResumeRecording") << tr("SetTransitionDuration")
		      << tr("SetCurrentProfile")
		      << tr("SetCurrentSceneCollection") << tr("ResetSceneItem")
		      << tr("SetTextGDIPlusText") << tr("SetBrowserSourceURL")
		      << tr("ReloadBrowserSource") << tr("TakeSourceScreenshot")
		      << tr("EnableSourceFilter") << tr("DisableSourceFilter")
		      << tr("ToggleSourceFilter");
		break;
	case 1:
		//Fader

		items << tr("SetVolume") << tr("SetSyncOffset")
		      << tr("SetSourcePosition") << tr("SetSourceRotation")
		      << tr("SetSourceScale") << tr("SetTransitionDuration")
		      << tr("SetGainFilter");
		break;
	case 2:
		//OSC
		break;
	case 4:
		//Soundboard
		break;
	}
	return items;
}
