/*
 Navicat Premium Data Transfer

 Source Server         : 192.168.1.9
 Source Server Type    : MySQL
 Source Server Version : 80025
 Source Host           : 192.168.1.9:3306
 Source Schema         : XEngine_Storage

 Target Server Type    : MySQL
 Target Server Version : 80025
 File Encoding         : 65001

 Date: 17/06/2021 17:19:07
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for XStorage_File
-- ----------------------------
DROP TABLE IF EXISTS `XStorage_File`;
CREATE TABLE `XStorage_File`  (
  `ID` int NOT NULL AUTO_INCREMENT COMMENT 'ID序号',
  `FilePath` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '文件路径',
  `FileName` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '文件名称',
  `FileHash` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '文件HASH',
  `FileUser` varchar(260) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '文件所属用户',
  `FileSize` bigint NOT NULL COMMENT '文件大小',
  `FileTime` datetime NOT NULL COMMENT '插入时间',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

SET FOREIGN_KEY_CHECKS = 1;
