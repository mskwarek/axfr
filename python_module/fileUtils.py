import os

class WriteUtils(object):
    def get_axfr_output_path(self):
        return self.answ_folder
    
    def get_log_output_path(self):
        return self.logfile
    
    def __init__(self, directory):
        self.logfile = create_output_folder_if_needed('log')
        self.answ_folder = create_output_folder_if_needed('ans')
        self.directory = directory
    def create_output_folder_if_needed(self, folder_to_create):
        #directory = parse_arguments().output
        if not os.path.exists(self.directory+'/'+folder_to_create):
            os.makedirs(self.directory+'/'+folder_to_create)
        #return self.directory+'/'+folder_to_create

    def write_result_to_file(self, returnedVec, domain):
        with open(self, get_axfr_output_path()+'/'+str(domain), 'w') as fi:
            fi.write("\n".join(returnedVec))
        
