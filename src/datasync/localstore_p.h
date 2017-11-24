#ifndef LOCALSTORE_P_H
#define LOCALSTORE_P_H

#include "qtdatasync_global.h"
#include "defaults.h"
#include "logger.h"
#include "exception.h"

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QCache>

#include <QtSql/QSqlDatabase>

#include <QReadWriteLock>

namespace QtDataSync {

class Q_DATASYNC_EXPORT LocalStore : public QObject
{
	Q_OBJECT

public:
	explicit LocalStore(QObject *parent = nullptr);
	explicit LocalStore(const QString &setupName, QObject *parent = nullptr);

	quint64 count(const QByteArray &typeName);
	QStringList keys(const QByteArray &typeName);

	QList<QJsonObject> loadAll(const QByteArray &typeName);
	QJsonObject load(const ObjectKey &key);
	void save(const ObjectKey &key, const QJsonObject &data);
	bool remove(const ObjectKey &key);
	QList<QJsonObject> find(const QByteArray &typeName, const QString &query);

Q_SIGNALS:
	void dataChanged(const ObjectKey &key);

private:
	static QReadWriteLock globalLock;

	Defaults defaults;
	Logger *logger;

	DatabaseRef database;

	QHash<QByteArray, QString> tableNameCache;
	QCache<ObjectKey, QJsonObject> dataCache;//TODO clear on changed

	QString getTable(const QByteArray &typeName, bool allowCreate = false);
	QDir typeDirectory(const QString &tableName, const ObjectKey &key);
	QJsonObject readJson(const QString &tableName, const QString &fileName, const ObjectKey &key, int *costs = nullptr);
};

class Q_DATASYNC_EXPORT LocalStoreException : public Exception
{
public:
	LocalStoreException(const Defaults &defaults, const ObjectKey &key, const QString &context, const QString &message);

	ObjectKey key() const;
	QString context() const;

	QString qWhat() const override;
	void raise() const override;
	QException *clone() const override;

protected:
	LocalStoreException(const LocalStoreException * const other);

	const ObjectKey _key;
	const QString _context;
};

class Q_DATASYNC_EXPORT NoDataException : public Exception
{
public:
	NoDataException(const Defaults &defaults, const ObjectKey &key);

	ObjectKey key() const;

	QString qWhat() const override;
	void raise() const override;
	QException *clone() const override;

protected:
	NoDataException(const NoDataException * const other);

	const ObjectKey _key;
};

}

#endif // LOCALSTORE_P_H