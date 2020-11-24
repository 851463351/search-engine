#ifndef __WD_TASK_HPP__
#define __WD_TASK_HPP__
#include <functional>

namespace wd
{

using Task = std::function<void()>;

}//end of namespace wd

#endif
