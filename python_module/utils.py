import os

class WriteUtils(object):
    def __init__(self, directory):
        self.directory = directory
        self.ans_folder = self.create_output_folder_if_needed('ans')
        self.logfile = self.create_output_folder_if_needed('.')+'/log'
        
    def get_axfr_output_path(self):
        return self.ans_folder
    
    def get_log_output_path(self):
        return self.logfile
    
    def create_output_folder_if_needed(self, folder_to_create):
        if not os.path.exists(self.directory+'/'+folder_to_create):
            os.makedirs(self.directory+'/'+folder_to_create)
        return self.directory+'/'+folder_to_create
        
    def write_result_to_file(self, returnedVec, domain):
        with open(self.get_axfr_output_path()+'/'+str(domain), 'w') as fi:
            fi.write("\n".join(returnedVec))
                                                            
