#include "page.h"

int Page::_currentPage = MAIN_PAGE;

int Page::getCurrentPage()
{
  return _currentPage;
}

void Page::setCurrentPage(int page)
{
  _currentPage = page;
}