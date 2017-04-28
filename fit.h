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

#ifndef FIT_H
#define FIT_H

#include <QFlag>
#include <QSize>
#include <QFont>

#ifdef QT_WIDGETS_LIB
class QWidget;
#endif

#if defined(QT_QML_LIB)
class QQmlContext;
#endif

#if defined(QT_QUICK_LIB) && defined(QT_QML_LIB)
class QQuickItem;
#endif

class QObject;
class QString;

namespace Fit
{
		enum FitModes {
			Width = 0x0001,
			Height = 0x0002,
			WidthHeight = Width | Height,
			Font = 0x0004,
			All = WidthHeight | Font
		};
		Q_DECLARE_FLAGS(FitMode, FitModes)
		Q_DECLARE_OPERATORS_FOR_FLAGS(Fit::FitMode)

		float ratio();
		void init(const int width, const int height, const float dpi);
		float fit(const float a);
		const QFont fit(const QFont& font);
		void fit(QObject* const object, const char* name);
		const QSize fit(const QSize& size, const FitMode mode = WidthHeight);
		#if defined(QT_QML_LIB)
		void fit(QObject* const object, const QString& name, QQmlContext* const context);
		#endif
		#if defined(QT_WIDGETS_LIB)
		void fit(QWidget* const widget, const FitMode mode = All, const bool layout = false);
		#endif
		#if defined(QT_QUICK_LIB) && defined(QT_QML_LIB)
		void fit(QQuickItem* const item, const FitMode mode = WidthHeight, const bool layout = false);
		#endif
}

#endif // FIT_H
