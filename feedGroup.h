#ifndef FEED_GROUP_H
#define FEED_GROUP_H

#include <list>
#include "feed.h"

class FeedGroup : public std::list<Feed> {
	public:
		void addFeed(Feed const& feed) {push_back(feed);}
};
#endif
