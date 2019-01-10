#include <aispeak.h>
#include <limits.h>
#include <random>

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QDebug>

#define URL "https://api.api.ai/v1/tts?v=20150910&text="

static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_int_distribution<int> rand_dist(INT_MIN, INT_MIN);

class AiSpeakPrivate
{
	public:
		QString id = AiSpeak::generateRandomId();
		QNetworkAccessManager* manager = new QNetworkAccessManager;
		QNetworkReply* reply;
};

AiSpeak::AiSpeak(QObject *parent)
	: QObject(parent)
	, _d(new AiSpeakPrivate)
	, m_language("en")
{
}

AiSpeak::~AiSpeak()
{
	delete _d;
}

const QString& AiSpeak::token() const
{
	return m_token;
}

void AiSpeak::setToken(const QString& token)
{
	m_token = token;
}

const QString& AiSpeak::language() const
{
	return m_language;
}

void AiSpeak::setLanguage(const QString& language)
{
	m_language = language;
}

void AiSpeak::speak(const QString& text)
{
	auto url(QUrl(URL + text));
	QNetworkRequest http(url);
	http.setRawHeader("Authorization", QString("Bearer " + m_token).toStdString().c_str());
	http.setRawHeader("Accept-Language", m_language.toStdString().c_str());

	_d->reply = _d->manager->get(http);

    connect(_d->reply, &QNetworkReply::sslErrors, _d->reply, QOverload<>::of(&QNetworkReply::ignoreSslErrors));
    connect(_d->reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &AiSpeak::handleError);
    connect(_d->reply, &QNetworkReply::finished, this, &AiSpeak::handleResponse);
}

void AiSpeak::handleResponse()
{
	auto data = _d->reply->readAll();
	if (_d->reply->error() == QNetworkReply::NoError)
		emit readySpeak(data);
	_d->reply->deleteLater();
}

void AiSpeak::handleError(QNetworkReply::NetworkError error)
{
	qWarning() << "AiSpeak::speak() :" << error;
	_d->reply->deleteLater();
	emit errorOccurred();
}

QString AiSpeak::generateRandomId()
{
	auto s4 = [] {
        return QString::number(65536 + (rand_dist(mt) % 65536), 16).remove(0, 1);
    };
    return (s4() + s4() + "-" + s4() + "-" + s4() + "-" + s4() + "-" + s4() + s4() + s4());
}