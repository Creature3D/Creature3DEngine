#ifndef _CINBETWEENACTIVITY_H
#define _CINBETWEENACTIVITY_H

#include "rbody/global.h"
#include <string>

namespace rbody {

/// Contains in-between activity (transition activity between to activities) for two activities
class REPLICANTBODY_API InBetweenActivity {

    public:

        /// Constructor
        InBetweenActivity() {};

        /// Destructor
        ~InBetweenActivity() {};

        /// Set start activity name
        /**
         * \param activity_name is name of activity
         **/
        void setStartActivity(const std::string& activity_name) { m_start_activity = activity_name; };

        /// Get start activity name
        std::string getStartActivity() { return m_start_activity; };

        /// Set end activity name
        /**
         * \param activity_name is name of activity
         **/
        void setEndActivity(const std::string& activity_name) { m_end_activity = activity_name; };

        /// Get end activity name
        std::string getEndActivity() { return m_end_activity; };

        /// Set in-between activity name
        /**
         * \param activity_name is name of activity
         **/
        void setInBetweenActivity(const std::string& activity_name) { m_in_between_activity = activity_name; };

        /// Get in-between activity name
        std::string getInBetweenActivity() { return m_in_between_activity; };

    private:

        /// Name of start activity
        std::string m_start_activity;

        /// Name of end activity
        std::string m_end_activity;

        /// Name of in-between activity
        std::string m_in_between_activity;
};

} // namespace end

#endif // _CINBETWEENACTIVITY_H
