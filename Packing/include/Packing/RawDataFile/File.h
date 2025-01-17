#ifndef PACKING_RAWDATAFILE_FILE_H_
#define PACKING_RAWDATAFILE_FILE_H_

#include "Framework/Configure/Parameters.h"
#include "Framework/Event.h"
#include "Framework/RunHeader.h"
#include "Packing/RawDataFile/EventPacket.h"
#include "Packing/Utility/Reader.h"
#include "Packing/Utility/Writer.h"

namespace packing {
namespace rawdatafile {

/**
 * The raw data file object
 */
class File {
 public:
  /**
   * General file constructor
   *
   * @param[in] params parameters use to configure this file
   * @param[in] filename name of the file to read/write
   */
  File(const framework::config::Parameters& params);

  /**
   * Connect the passed event bus to this event file.
   */
  bool connect(framework::Event& event);

  /**
   * Load the next event into our connected event bus.
   */
  bool nextEvent();

  /**
   * Write the run header
   */
  void writeRunHeader(ldmx::RunHeader& header);

  /// close this file
  void close();

 private:
  /// are we reading or writing?
  bool is_output_;
  /// the event bus object name of the ECal encoded data
  std::string ecal_object_name_;
  /// the event bus object name of the HCal encoded data
  std::string hcal_object_name_;
  /// the event bus object name of the Tracker encoded data
  std::string tracker_object_name_;
  /// the event bus object name of the Trigger Pad encoded data
  std::string triggerpad_object_name_;
  /// for writing, we also should be able to specify a pass name
  std::string pass_name_;
  /// for writing, do we skip subsystems if their raw object is unavailable
  bool skip_unavailable_;

 private:
  /// number of entries in the file
  uint32_t entries_{0};
  /// current entry index (may not be same as event number)
  uint32_t i_entry_{0};
  /// handle to the event bus we are reading from or writing to
  framework::Event* event_{nullptr};
  /// run number corresponding to this file of raw data
  uint32_t run_;
  /// utility class for reading binary data files
  utility::Reader reader_;
  /// utility class for writing binary data files
  utility::Writer writer_;
  /// crc calculator for output mode
  utility::CRC crc_;
};  // File

}  // namespace rawdatafile
}  // namespace packing

#endif  // PACKING_RAWDATAFILE_FILE_H_
