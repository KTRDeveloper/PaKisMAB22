if [[ "${COMP_S3_PROBLEM_PATH}" == *".xz" ]]; then
  aws s3 cp s3://${S3_BKT}/${COMP_S3_PROBLEM_PATH} test.cnf.xz
  unxz test.cnf.xz
else
  aws s3 cp s3://${S3_BKT}/${COMP_S3_PROBLEM_PATH} test.cnf
fi

./PaKis -c=24 -max-memory=256 test.cnf
