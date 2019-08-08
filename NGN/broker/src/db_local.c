#include "db.h"
MDB_env* mdb_env_;

int MDB_CHECK(int mdb_status)
{
    if (mdb_status != MDB_SUCCESS)
    {
        zsys_warning("MDB_CHECK error is %s",mdb_strerror(mdb_status));
    }
    return mdb_status;
}

uint32_t put_data_local(char *key,int key_size,char *value,int value_size,int db_flag)
{
    assert(key);
    assert(value);

    int rc = -1;
    //MDB_env* mdb_env_;
    MDB_dbi mdb_dbi_ ;
    MDB_txn* mdb_txn = NULL;
    MDB_val mdb_key, mdb_value;
    char db_path[DB_PATH_MAX_LEN] = {0};


    if(db_flag == DB_FLAG_LOCAL)
    {
    	   strcpy(db_path,DB_FILE_PATH_LOCAL);
    }
    else
    {
    	   strcpy(db_path,DB_FILE_PATH_REMOTE);    
    }

    rc = MDB_CHECK(mdb_env_create(&mdb_env_));
    if(rc != 0)
    {
    	  zsys_warning("put_data_local mdb_env_create error is %d",rc);
    	  goto END;
    }
    rc = MDB_CHECK(mdb_env_set_mapsize(mdb_env_, LMDB_MAP_SIZE));
    if(rc != 0)
    {
    	  zsys_warning("put_data_local mdb_env_set_mapsize error is %d",rc);
    	  goto END;
    }
    //create db file
    if(!zfile_exists(db_path))
    {
        zsys_warning("put_data_local zfile_exists error mkdir");
        zfile_mkdir(db_path);
    }

    rc = MDB_CHECK(mdb_env_open(mdb_env_, db_path, MDB_NOSYNC, 0664));
    if(rc != 0)
    {
         zsys_warning("put_data_local mdb_env_open error is %d",rc);		
    	  goto END;
    }

    rc = MDB_CHECK(mdb_txn_begin(mdb_env_, NULL, 0, &mdb_txn));
    if(rc != 0)
    {	
         zsys_warning("put_data_local mdb_txn_begin error is %d",rc); 		
    	  goto END;
    }


    rc = MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi_));
    if(rc != 0)
    {
    	  zsys_warning("put_data_local mdb_dbi_open error is %d",rc); 
    	  goto END;
    }

    mdb_key.mv_data = key;
    mdb_key.mv_size = key_size;

    mdb_value.mv_data = value;
    mdb_value.mv_size = value_size;


    rc = MDB_CHECK(mdb_put(mdb_txn, mdb_dbi_, &mdb_key, &mdb_value, MDB_NODUPDATA));
    if(rc != 0)
    {      
    	 zsys_warning("put_data_local mdb_put error is %d",rc); 
    	  goto END;
    }

    rc = MDB_CHECK(mdb_txn_commit(mdb_txn));
    if(rc != 0)
    {
         if(rc == MDB_PAGE_NOTFOUND)
         {
         	char cmd[64] = {0};
		sprintf(cmd,"rm %s -rf",db_path);
         	system(cmd);
         }
    	  zsys_warning("put_data_local mdb_txn_commiterror is %d",rc); 
    	  goto END;
    }

    rc = NNGN_ERR_NONE;
END:
    mdb_txn_abort(mdb_txn);
    mdb_dbi_close(mdb_env_, mdb_dbi_);
    mdb_env_close(mdb_env_);
    return rc;
}

uint32_t get_data_local(char *key,int key_size,char **value,int *value_size,int db_flag)
{
    //MDB_env* mdb_env_;
    MDB_txn* mdb_txn = NULL;
    MDB_dbi mdb_dbi_;
    MDB_val mdb_key, mdb_value;
    MDB_cursor* mdb_cursor;
    int rc = -1;
    char db_path[DB_PATH_MAX_LEN] = {0};

    if(db_flag == DB_FLAG_LOCAL)
    {
    	   strcpy(db_path,DB_FILE_PATH_LOCAL);
    }
    else
    {
    	   strcpy(db_path,DB_FILE_PATH_REMOTE);    
    }

    rc = MDB_CHECK(mdb_env_create(&mdb_env_));
    if(rc != 0)
    {
    	   zsys_warning("get_data_local mdb_env_create is %d",rc); 
    	  goto END;
    }
    rc = MDB_CHECK(mdb_env_set_mapsize(mdb_env_, LMDB_MAP_SIZE));
    if(rc != 0)
    {
         zsys_warning("get_data_local mdb_env_set_mapsize is %d",rc); 		
    	  goto END;
    }

    //create db file
    if(!zfile_exists(db_path))
    {
        zsys_warning("get_data_local zfile_exists dir is not exist");
        rc = -1;
        goto END;
    }

    rc = MDB_CHECK(mdb_env_open(mdb_env_, db_path, MDB_RDONLY, S_IRUSR));
    if(rc != 0)
    {
    	  zsys_warning("get_data_local mdb_env_open error rc is %d",rc);
    	  goto END;
    }
	
    rc = MDB_CHECK(mdb_txn_begin(mdb_env_, NULL, MDB_RDONLY, &mdb_txn));
    if(rc != 0)
    {
    	  zsys_warning("get_data_local mdb_txn_begin error rc is %d",rc);
    	  goto END;
    }
	
    rc = MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi_));
    if(rc != 0)
    {
    	  zsys_warning("get_data_local mdb_dbi_open error rc is %d",rc);
    	  goto END;
    }
	
    rc = MDB_CHECK(mdb_cursor_open(mdb_txn, mdb_dbi_, &mdb_cursor));
    if(rc != 0)
    {
    	  zsys_warning("get_data_local mdb_cursor_open error rc is %d",rc);
    	  goto END;
    }
    mdb_key.mv_data = key;
    mdb_key.mv_size = key_size;

    rc = mdb_get(mdb_txn, mdb_dbi_, &mdb_key, &mdb_value);
    if(rc == 0)
    {
        *value_size = mdb_value.mv_size;
        *value = calloc(mdb_value.mv_size,1);
        memcpy(*value,mdb_value.mv_data,*value_size);
    }
    else
    {
    	rc = -1;
    }
    mdb_dbi_close(mdb_env_, mdb_dbi_);	
    mdb_cursor_close(mdb_cursor);
    mdb_txn_abort(mdb_txn);	
END:
    mdb_env_close(mdb_env_);
    return rc;
}

uint32_t check_db()
{
    //MDB_env* mdb_env_;
    MDB_txn* mdb_txn;
    MDB_dbi mdb_dbi_;
    MDB_val mdb_key, mdb_value;
    MDB_cursor* mdb_cursor;
    int rc = -1;

    rc = MDB_CHECK(mdb_env_create(&mdb_env_));
    assert(rc == 0);
    rc = MDB_CHECK(mdb_env_set_mapsize(mdb_env_, LMDB_MAP_SIZE));
    assert(rc == 0);
    //create db file

    rc = MDB_CHECK(mdb_env_open(mdb_env_, DB_FILE_PATH_LOCAL, MDB_NOSYNC, S_IRUSR));
    if(rc != 0)
    {
    	 return rc;
    }
    rc = MDB_CHECK(mdb_txn_begin(mdb_env_, NULL, MDB_NOSYNC, &mdb_txn));
    if(rc != 0)
    {
    	 return rc;
    }
    rc = MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi_));
    if(rc != 0)
    {
    	 return rc;
    }
    rc = MDB_CHECK(mdb_cursor_open(mdb_txn, mdb_dbi_, &mdb_cursor));
    if(rc != 0)
    {
    	 return rc;
    }
 

    MDB_val key, data;	
    while ((rc = mdb_cursor_get(mdb_cursor, &key, &data, MDB_NEXT)) == 0) {
		printf("key: %s ,  size %d\n",
			(char *)key.mv_data,  
			 (int) data.mv_size); 
		   
	}
		
    //MDB_CHECK(mdb_page_flush(mdb_txn,0));
    mdb_cursor_close(mdb_cursor);
    mdb_txn_abort(mdb_txn);
    mdb_dbi_close(mdb_env_, mdb_dbi_);
    mdb_env_close(mdb_env_);
    return rc;
}



