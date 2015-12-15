#include "progress.hpp"

#include "download.hpp"
#include "resource.hpp"
#include "transaction.hpp"

static const char *TITLE = "ReaPack: Download in progress";

using namespace std;

Progress::Progress()
  : Dialog(IDD_PROGRESS_DIALOG),
    m_transaction(nullptr), m_label(nullptr), m_progress(nullptr),
    m_done(0), m_total(0)
{
}

void Progress::setTransaction(Transaction *t)
{
  m_transaction = t;

  m_done = 0;
  m_total = 0;
  m_currentName.clear();

  if(!m_transaction)
    return;

  SetWindowText(m_label, "Initializing...");

  m_transaction->downloadQueue()->onPush(
    bind(&Progress::addDownload, this, placeholders::_1));
}

void Progress::onInit()
{
  m_label = getItem(IDC_LABEL);
  m_progress = GetDlgItem(handle(), IDC_PROGRESS);
}

void Progress::onCommand(WPARAM wParam, LPARAM)
{
  const int commandId = LOWORD(wParam);

  switch(commandId) {
  case IDCANCEL:
    if(m_transaction)
      m_transaction->cancel();

    // don't wait until the current downloads are finished
    // before getting out of the user way
    hide();
    break;
  }
}

void Progress::addDownload(Download *dl)
{
  m_total++;
  updateProgress();

  dl->onStart([=] {
    m_currentName = dl->name();
    updateProgress();
  });

  dl->onFinish([=] {
    m_done++;
    updateProgress();
  });
}

void Progress::updateProgress()
{
  const string text = "Downloading " +
    to_string(min(m_done + 1, m_total)) + " of " +
    to_string(m_total) + ": " + m_currentName;

  SetWindowText(m_label, text.c_str());

  const double pos = (double)m_done / m_total;
  const int percent = (int)(pos * 100);
  const string title = string(TITLE) + " (" + to_string(percent) + "%)";

  SendMessage(m_progress, PBM_SETPOS, percent, 0);
  SetWindowText(handle(), title.c_str());
}