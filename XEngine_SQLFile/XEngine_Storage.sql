/*
 Navicat Premium Data Transfer

 Source Server         : QCloud
 Source Server Type    : MySQL
 Source Server Version : 80032 (8.0.32-0ubuntu0.22.04.2)
 Source Host           : 42.194.178.57:3306
 Source Schema         : XEngine_Storage

 Target Server Type    : MySQL
 Target Server Version : 80032 (8.0.32-0ubuntu0.22.04.2)
 File Encoding         : 65001

 Date: 19/04/2023 13:46:21
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for XStorage_File
-- ----------------------------
DROP TABLE IF EXISTS `XStorage_File`;
CREATE TABLE `XStorage_File`  (
  `ID` int NOT NULL AUTO_INCREMENT COMMENT 'ID序号',
  `BuckKey` varchar(260) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL COMMENT '路径KEY',
  `FilePath` varchar(260) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '文件路径',
  `FileName` varchar(260) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '文件名称',
  `FileHash` varchar(260) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL COMMENT '文件HASH',
  `FileUser` varchar(260) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT NULL COMMENT '文件所属用户',
  `FileSize` bigint NOT NULL COMMENT '文件大小',
  `FileTime` datetime NOT NULL COMMENT '插入时间',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

SET FOREIGN_KEY_CHECKS = 1;