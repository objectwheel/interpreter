/*****************************************************************************
**
** Project Fit: Cross-Platform Dpi Based Qt Visual Component Scaling Library
** Copyright (C) 2016 Ömer Göktaş
** Contact: omer@omergoktas.com
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, version 3 of the License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/

#include <fit.h>

#include <QFontInfo>
#include <QGuiApplication>
#include <QScreen>

#if defined(QT_WIDGETS_LIB)
#include <QWidget>
#endif

#if defined(QT_QML_LIB)
#include <QQmlProperty>
#endif

#if defined(QT_QUICK_LIB) && defined(QT_QML_LIB)
#include <QQuickItem>
#include <QQmlEngine>
#endif

static float m_ratio = 1.0;

float Fit::fit(const float a)
{
	return a * m_ratio;
}

float Fit::ratio()
{
	return m_ratio;
}

const QFont Fit::fit(const QFont& font)
{
	QFont f(font);
	f.setPixelSize(fit(font.pixelSize()));
	return f;
}

void Fit::fit(QObject* const object, const char* property)
{
	object->setProperty(property, fit(object->property(property).toReal()));
}

void Fit::init(const int width, const int height, const float dpi)
{
	/* Calculate dpi scale ratio */
	auto screen = QGuiApplication::primaryScreen();
	float sdpi = screen->physicalDotsPerInch();
	m_ratio = sdpi / dpi;

	/* Decrease ratio if device screen size lower than ref screen (in inch) */
	int w = qMax(width, height);
	int h = qMin(width, height);
	int sw = qMax(screen->size().width(), screen->size().height());
	int sh = qMin(screen->size().width(), screen->size().height());
	float r = qMin((sw / sdpi) / (w / dpi), (sh / sdpi) / (h / dpi));
	if (r < 1.0) {
		m_ratio *= r;
	}
}

const QSize Fit::fit(const QSize& size, const FitMode mode)
{
	QSize ret;
	if (mode == Width) {
		ret = QSize(fit(size.width()), size.height());
	} else if (mode == Height) {
		ret = QSize(size.width(), fit(size.height()));
	} else if (mode == WidthHeight) {
		ret = QSize(fit(size.width()), fit(size.height()));
	} else {
		Q_ASSERT_X(0, "Fit::fit() ", "Wrong FitMode choice.");
	}
	return ret;
}

#if defined(QT_QML_LIB)
void Fit::fit(QObject* const object, const QString& property, QQmlContext* const context)
{
	QQmlProperty::write(object, property, fit(QQmlProperty::read(object, property, context).toReal()), context);
}
#endif

#if defined(QT_WIDGETS_LIB)
void Fit::fit(QWidget* const widget, const FitMode mode, const bool layout)
{
	if ((mode == Width) && !layout) {
		widget->resize(QSize(fit(widget->width()), widget->height()));
	} else if ((mode == Height) && !layout) {
		widget->resize(QSize(widget->width(), fit(widget->height())));
	} else if ((mode == WidthHeight) && !layout) {
		widget->resize(QSize(fit(widget->width()), fit(widget->height())));
	} else if ((mode == Font) && !layout) {
		widget->setFont(fit(widget->font()));
	} else if ((mode == Font) && layout) {
		widget->setFont(fit(widget->font()));
	} else if ((mode == All) && !layout) {
		widget->resize(QSize(fit(widget->width()), fit(widget->height())));
		widget->setFont(fit(widget->font()));
	} else if ((mode == (Width | Font)) && !layout) {
		widget->resize(QSize(fit(widget->width()), widget->height()));
		widget->setFont(fit(widget->font()));
	} else if ((mode == (Height | Font)) && !layout) {
		widget->resize(QSize(widget->width(), fit(widget->height())));
		widget->setFont(fit(widget->font()));
	} else if ((mode == Width) && layout) {
		widget->setMinimumWidth(fit(widget->width()));
		widget->setMaximumWidth(fit(widget->width()));
	} else if ((mode == Height) && layout) {
		widget->setMinimumHeight(fit(widget->height()));
		widget->setMaximumHeight(fit(widget->height()));
	} else if ((mode == WidthHeight) && layout) {
		widget->setMinimumSize(QSize(fit(widget->width()), fit(widget->height())));
		widget->setMaximumSize(QSize(fit(widget->width()), fit(widget->height())));
	} else if ((mode == All) && layout) {
		widget->setMinimumSize(QSize(fit(widget->width()), fit(widget->height())));
		widget->setMaximumSize(QSize(fit(widget->width()), fit(widget->height())));
		widget->setFont(fit(widget->font()));
	} else if ((mode == (Width | Font)) && layout) {
		widget->setMinimumWidth(fit(widget->width()));
		widget->setMaximumWidth(fit(widget->width()));
		widget->setFont(fit(widget->font()));
	} else if ((mode == (Height | Font)) && layout) {
		widget->setMinimumHeight(fit(widget->height()));
		widget->setMaximumHeight(fit(widget->height()));
		widget->setFont(fit(widget->font()));
	} else {
		Q_ASSERT_X(0, "Fit::fit() ", "Wrong FitMode choice.");
	}
}
#endif

#if defined(QT_QUICK_LIB) && defined(QT_QML_LIB)
void Fit::fit(QQuickItem* const item, const FitMode mode, const bool layout)
{
	if ((mode == Width) && !layout) {
		item->setWidth(fit(item->width()));
		item->setHeight(item->height());
	} else if ((mode == Height) && !layout) {
		item->setWidth(item->width());
		item->setHeight(fit(item->height()));
	} else if ((mode == WidthHeight) && !layout) {
		item->setWidth(fit(item->width()));
		item->setHeight(fit(item->height()));
	} else if ((mode == Width) && layout) {
		QQmlProperty::write(item, "Layout.maximumWidth", fit(item->width()), qmlContext(item));
		QQmlProperty::write(item, "Layout.minimumWidth", fit(item->width()), qmlContext(item));
	} else if ((mode == Height) && layout) {
		QQmlProperty::write(item, "Layout.maximumHeight", fit(item->height()), qmlContext(item));
		QQmlProperty::write(item, "Layout.minimumHeight", fit(item->height()), qmlContext(item));
	} else if ((mode == WidthHeight) && layout) {
		QQmlProperty::write(item, "Layout.maximumWidth", fit(item->width()), qmlContext(item));
		QQmlProperty::write(item, "Layout.minimumWidth", fit(item->width()), qmlContext(item));
		QQmlProperty::write(item, "Layout.maximumHeight", fit(item->height()), qmlContext(item));
		QQmlProperty::write(item, "Layout.minimumHeight", fit(item->height()), qmlContext(item));
	} else {
		Q_ASSERT_X(0, "Fit::fit() ", "Wrong FitMode choice.");
	}
}
#endif
