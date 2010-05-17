/**
 * Copyright (c) 2010-2010 Andrey AndryBlack Kunitsyn
 * email:support.andryblack@gmail.com
 *
 * Report bugs and download new versions at http://code.google.com/p/fontbuilder
 *
 * This software is distributed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "fontbuilder.h"
#include "ui_fontbuilder.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QSettings>
#include <QMetaProperty>
#include <QDir>
#include <QMessageBox>

#include "fontconfig.h"
#include "fontrenderer.h"
#include "layoutconfig.h"
#include "layoutdata.h"
#include "layouterfactory.h"
#include "outputconfig.h"
#include "exporterfactory.h"
#include "imagewriterfactory.h"


FontBuilder::FontBuilder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FontBuilder)
{
    ui->setupUi(this);



    m_font_config = new FontConfig(this);
    bool font_config_block = m_font_config->blockSignals(true);
    connect(m_font_config,SIGNAL(nameChanged()),this,SLOT(onFontNameChanged()));
    connect(m_font_config,SIGNAL(sizeChanged()),this,SLOT(onFontNameChanged()));

    m_font_renderer = new FontRenderer(this,m_font_config);

    connect(m_font_renderer,SIGNAL(imagesChanged()),this,SLOT(onRenderedChanged()));

    m_layout_config = new LayoutConfig(this);
    m_layout_data = new LayoutData(this);

    connect(m_layout_data,SIGNAL(layoutChanged()),this,SLOT(onLayoutChanged()));

    m_layouter = 0;
    m_layouter_factory = new LayouterFactory(this);

    bool b = ui->comboBoxLayouter->blockSignals(true);
    ui->comboBoxLayouter->clear();
    ui->comboBoxLayouter->addItems(m_layouter_factory->names());

    m_output_config = new OutputConfig(this);

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    readConfig(settings,"fontconfig",m_font_config);
    readConfig(settings,"layoutconfig",m_layout_config);
    readConfig(settings,"outputconfig",m_output_config);
    ui->checkBoxDrawGrid->setChecked(settings.value("draw_grid").toBool());
    connect(ui->checkBoxDrawGrid,SIGNAL(toggled(bool)),this,SLOT(onLayoutChanged()));

    ui->frameCharacters->setConfig(m_font_config);
    ui->frameFontOptions->setConfig(m_font_config);
    if (!m_layout_config->layouter().isEmpty()) {
        for (int i=0;i<ui->comboBoxLayouter->count();i++)
            if (ui->comboBoxLayouter->itemText(i)==m_layout_config->layouter())
                ui->comboBoxLayouter->setCurrentIndex(i);
    }
    ui->frameLayoutConfig->setConfig(m_layout_config);


    m_exporter_factory = new ExporterFactory(this);
    ui->frameOutput->setExporters(m_exporter_factory->names());

    m_image_writer_factory = new ImageWriterFactory(this);
    ui->frameOutput->setImageWriters(m_image_writer_factory->names());

    ui->comboBoxLayouter->blockSignals(b);
    this->on_comboBoxLayouter_currentIndexChanged(
            ui->comboBoxLayouter->currentText());

    ui->frameOutput->setConfig(m_output_config);
    ui->frameFontSelect->setConfig(m_font_config);
    m_font_config->blockSignals(font_config_block);
    m_font_config->emmitChange();
}

FontBuilder::~FontBuilder()
{
    delete ui;
}


void FontBuilder::closeEvent(QCloseEvent *event)
 {
     QSettings settings;
     settings.setValue("geometry", saveGeometry());
     saveConfig(settings,"fontconfig",m_font_config);
     saveConfig(settings,"layoutconfig",m_layout_config);
     saveConfig(settings,"outputconfig",m_output_config);
     settings.setValue("draw_grid",ui->checkBoxDrawGrid->isChecked());
     QMainWindow::closeEvent(event);
 }

void FontBuilder::saveConfig(QSettings& settings,
                             const QString& name,
                             const QObject* object) {
    settings.beginGroup(name);
    const QMetaObject *metaobject = object->metaObject();
     int count = metaobject->propertyCount();
     for (int i=0; i<count; ++i) {
         QMetaProperty metaproperty = metaobject->property(i);
         const char *name = metaproperty.name();
         QVariant value = object->property(name);
         settings.setValue(name,value);
     }
    settings.endGroup();
}

void FontBuilder::readConfig(QSettings& settings,
                             const QString& name,
                             QObject* object) {
    settings.beginGroup(name);
    const QMetaObject *metaobject = object->metaObject();
     int count = metaobject->propertyCount();
     for (int i=0; i<count; ++i) {
         QMetaProperty metaproperty = metaobject->property(i);
         const char *name = metaproperty.name();
         if (settings.contains(name))
             object->setProperty(name,settings.value(name));
      }
    settings.endGroup();
}

void FontBuilder::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FontBuilder::fontParametersChanged() {
    qDebug("font parameters changed");
}

void FontBuilder::on_comboBoxLayouter_currentIndexChanged(QString name)
{
    if (name.isEmpty()) return;
    if (m_layouter) {
        delete m_layouter;
        m_layouter = 0;
    }
    m_layouter = m_layouter_factory->build(name,this);
    if (m_layouter) {
        m_layouter->setConfig(m_layout_config);
        m_layouter->setData(m_layout_data);
        connect(m_font_renderer,SIGNAL(imagesChanged(QVector<LayoutChar>)),
                m_layouter,SLOT(on_ReplaceImages(QVector<LayoutChar>)));
        m_layouter->on_ReplaceImages(m_font_renderer->rendered());
        m_layout_config->setLayouter(name);
    }
}

void FontBuilder::onRenderedChanged() {

}

void FontBuilder::onLayoutChanged() {
    QPixmap pixmap(m_layout_data->width(),m_layout_data->height());
    pixmap.fill(QColor(0,0,0,255));
    QPainter painter(&pixmap);
    foreach (const LayoutChar& c,m_layout_data->placed())
        m_font_renderer->placeImage(painter,c.symbol,
                                    c.x + m_layout_config->offsetLeft(),
                                    c.y + m_layout_config->offsetTop()
                                    );
    if (ui->checkBoxDrawGrid->isChecked()) {
        painter.setPen(QColor(255,0,255,255));
        if (m_layout_config->onePixelOffset())
            foreach (const LayoutChar& c,m_layout_data->placed()) {
                painter.drawRect(c.x-1,c.y-1,c.w,c.h);
            }
        else
            foreach (const LayoutChar& c,m_layout_data->placed()) {
                painter.drawRect(c.x,c.y,c.w-1,c.h-1);
            }
    }
    ui->label_Image->setPixmap(pixmap);
    ui->label_Image->repaint();
    ui->label_ImageSize->setText(tr("Image size: ")+
            QString().number(m_layout_data->width()) + "x" +
            QString().number(m_layout_data->height())
            );
}

void FontBuilder::onFontNameChanged() {
    QString name = m_font_config->family()+ "_" +
                   m_font_config->style()+ "_" +
                   QString().number(m_font_config->size());

    name = name.toLower().replace(" ","_");
    m_output_config->setImageName(name);
    m_output_config->setDescriptionName(name);
}

void FontBuilder::on_pushButtonWriteFont_clicked()
{
    QDir dir(m_output_config->path());
    if (m_output_config->writeImage()) {

        AbstractImageWriter* exporter = m_image_writer_factory->build(m_output_config->imageFormat(),this);
        if (!exporter) {
            QMessageBox msgBox;
            msgBox.setText(tr("Unknown exporter :")+m_output_config->descriptionFormat());
            msgBox.exec();
            return;
        }

        exporter->setData(m_layout_data,m_layout_config,m_font_renderer->data());

        QString filename = dir.filePath(m_output_config->imageName());
        filename+="."+exporter->extension();
        QFile file(this);
        file.setFileName(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            delete exporter;
            QMessageBox msgBox;
            msgBox.setText(tr("Error opening file :")+filename);
            msgBox.exec();
            return;
        }
        if (!exporter->Write(file)) {
            QMessageBox msgBox;
            msgBox.setText(tr("Error on save image :\n")+exporter->errorString()+"\nFile not writed.");
            msgBox.exec();
        }

        delete exporter;
    }
    if (m_output_config->writeDescription()) {
        AbstractExporter* exporter = m_exporter_factory->build(m_output_config->descriptionFormat(),this);
        if (!exporter) {
            QMessageBox msgBox;
            msgBox.setText(tr("Unknown exporter :")+m_output_config->descriptionFormat());
            msgBox.exec();
            return;
        }
        exporter->setFontConfig(m_font_config);
        exporter->setData(m_layout_data,m_font_renderer->data());
        QString filename = dir.filePath(m_output_config->descriptionName());
        filename+="."+exporter->getExtension();
        QByteArray data;
        if (!exporter->Write(data)) {
             QMessageBox msgBox;
             msgBox.setText(tr("Error on save description :\n")+exporter->getErrorString()+"\nFile not writed.");
             msgBox.exec();
         } else {
             QFile file(filename);
             if (file.open(QIODevice::WriteOnly)) {
                 file.write(data);
             } else {

             }
         }
        delete exporter;
    }
}


