#pragma once

// main router
enum PageId
{
  MAIN_PAGE,
  SETTINGS,
};

class Page
{
private:
  static int _currentPage; // one of PageId
public:
  static int getCurrentPage ();
  static void setCurrentPage (int page);
};