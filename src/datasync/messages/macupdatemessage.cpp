#include "macupdatemessage_p.h"
using namespace QtDataSync;

MacUpdateMessage::MacUpdateMessage(const QByteArray &cmac) :
	cmac(cmac)
{}

QDataStream &QtDataSync::operator<<(QDataStream &stream, const MacUpdateMessage &message)
{
	stream << message.cmac;
	return stream;
}

QDataStream &QtDataSync::operator>>(QDataStream &stream, MacUpdateMessage &message)
{
	stream.startTransaction();
	stream >> message.cmac;
	stream.commitTransaction();
	return stream;
}